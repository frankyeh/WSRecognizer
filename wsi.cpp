#include <functional>
#include "wsi.hpp"
#include "openslide.h"
#include "train_model.hpp"
#include "libs/gzip_interface.hpp"

wsi::wsi():handle(0)
{
}
wsi::~wsi()
{
    if(handle)
        openslide_close(handle);
}

bool wsi::can_open(const char* file_name)
{
    return openslide_can_open(file_name);
}

bool wsi::open(const char* file_name_)
{
    file_name = file_name_;
    handle = openslide_open(file_name_);
    if(!handle)
        return false;
    {
        int64_t w,h;
        openslide_get_level0_dimensions(handle,&w,&h);
        dim[0] = w;
        dim[1] = h;
        const char* ptr;
        ptr = openslide_get_property_value(handle,"openslide.mpp-x");
        // Microns per pixel in the X dimension of level 0.
        if(ptr)
            std::istringstream(ptr) >> pixel_size;
        else
            pixel_size = 0.5;
    }

    level = openslide_get_level_count(handle);
    dim_at_level.resize(level);
    r_at_level.resize(level);
    for(unsigned int index = 0;index < level;++index)
    {
        int64_t w,h;
        openslide_get_level_dimensions(handle,index,&w,&h);
        dim_at_level[index][0] = w;
        dim_at_level[index][1] = h;
        r_at_level[index] = openslide_get_level_downsample(handle,index);
    }

    // get map at 40 micron pixel size
    if(dim_at_level.size() == 1)
    {
        int scale = 0,mx = dim[0],my = dim[1];
        for(;mx > 1000;mx = mx/2,my = my/2)
            ++scale;
        map_image.resize(image::geometry<2>(mx,my));
        unsigned int thread_count = std::thread::hardware_concurrency();
        std::vector<openslide_t*> handles(thread_count);
        for(int j = 0;j < handles.size();++j)
            handles[j] = openslide_open(file_name.c_str());
        image::par_for2(dim[0]/512,[&](int dx,int id)
        {
            int x = dx*512;
            if(x >= dim[0])
                return;
            for(int y = 0;y < dim[1];y += 512)
            {
                image::color_image I(image::geometry<2>(std::min<int>(dim[0]-x-1,512),std::min<int>(dim[1]-y-1,512)));
                openslide_read_region(handles[id],(uint32_t*)&*I.begin(),x,y,0,I.width(),I.height());
                int px = x,py = y;
                for(int s = 0;s < scale;++s)
                {
                    image::downsampling(I);
                    px = px / 2;
                    py = py / 2;
                }
                image::draw(I,map_image,image::vector<2>(px,py));
            }
        });
        for(int j = 0;j < handles.size();++j)
            openslide_close(handles[j]);
        read_profile(map_image);
    }
    else
    {
        float zoom_ratio = 40.0/pixel_size;
        unsigned int level = openslide_get_best_level_for_downsample(handle,zoom_ratio);
        image::color_image I;
        I.resize(dim_at_level[level]);
        openslide_read_region(handle,(uint32_t*)&*I.begin(),0,0,level,I.width(),I.height());
        read_profile(I);
        map_image.resize(image::geometry<2>(dim_at_level[0][0]/zoom_ratio,dim_at_level[0][1]/zoom_ratio));
        image::scale(I,map_image);
    }




    map_mask.resize(map_image.geometry());
    {
        image::grayscale_image gI = map_image;
        image::basic_image<float,2> fI = gI;
        image::ml::k_means<double,unsigned char> k(10);
        k(fI.begin(),fI.end(),map_mask.begin());
        image::grayscale_image I(map_mask.geometry());
        for(int i = 0;i < map_mask.size();++i)
            I[i] = (map_mask[i] == map_mask[0] ? 0 : 1);
        I.swap(map_mask);
    }

    for(const char * const * str = openslide_get_property_names(handle);*str;++str)
    {
        property_name.push_back(*str);
        property_value.push_back(openslide_get_property_value(handle,*str));
    }
    for(const char * const * str = openslide_get_associated_image_names(handle);*str;++str)
    {
        associated_image_name.push_back(*str);
        int64_t w,h;
        openslide_get_associated_image_dimensions(handle,*str,&w,&h);
        associated_image.resize(associated_image.size()+1);
        associated_image.back().resize(image::geometry<2>(w,h));
        openslide_read_associated_image(handle,*str,(uint32_t*)&*associated_image.back().begin());
    }
    process_mask();
    return true;
}
void wsi::set_stain_scale(float stain1_scale,float stain2_scale)
{
    if(stain1_scale == 1.0 && stain2_scale == 1.0)
    {
        stain_scale = false;
        return;
    }
    stain_scale = true;
    image::matrix<3,3,float> s;
    s.identity();
    s[4] = stain1_scale;
    s[8] = stain2_scale;

    stain_scale_transform = color_m;
    stain_scale_transform *= s;
    stain_scale_transform *= color_m_inv;
    stain_scale = true;
}

void wsi::read_profile(const image::color_image& I)
{
    //get background
    image::vector<3> g;
    {
        image::color_image I(image::geometry<2>(100,100));
        read(I,0,0,0);
        for(int i = 0;i < I.size();++i)
        {
            g[0] += I[i][0];
            g[1] += I[i][1];
            g[2] += I[i][2];
        }
        g.normalize();
        //std::cout << g << std::endl;
    }
    g.normalize();
    color0_v = g;
    image::vector<3> b(1.0,0.0,0.0),nb;
    b = b-g*(b*g);
    nb = b.cross_product(g);
    image::basic_image<double,1> count(image::geometry<1>(720));
    // calculate color histogram
    for(int i = 0;i < I.size();++i)
    {
        image::vector<3> v(I[i].data);
        v -= g*(v*g);
        double l = v.length();
        if(l < 30.0)
            continue;
        int angle = std::round(std::atan2(nb*v,b*v)*180.0/3.14159265358979323846);
        if(angle < 0)
            angle += 360;
        if(angle >= 360)
            angle -= 360;
        count[angle] += l;
        count[angle+360] += l;
    }

    int local_max_count = 0;
    std::map<double,int,std::greater<double> > stain_map;
    do{
        local_max_count = 0;
        image::filter::gaussian(count);
        // correct boundary value
        count[0] = count[360];
        count[1] = count[361];
        count[719] = count[719-360];
        count[718] = count[718-360];
        stain_map.clear();
        for(int i = 1;i <= 360;++i)
            if(count[i] > count[i-1] && count[i] > count[i+1])
            {
                ++local_max_count;
                stain_map[count[i]] = i;
            }
        //std::cout << "max=";
        //for(auto i = stain_map.begin();i != stain_map.end();++i)
        //    std::cout << i->first << ":" << i->second << " ";
        //std::cout << std::endl;
    }while(local_max_count > 2);
    if(stain_map.empty())
        return;
    auto result =stain_map.begin();
    color1_count = result->first;
    color1_code = result->second;
    double angle1 = result->second*3.14159265358979323846/180;
    ++result;
    color2_count = result->first;
    color2_code = result->second;
    double angle2 = result->second*3.14159265358979323846/180;
    image::vector<3> v1,v2;
    color1_v = b*std::cos(angle1) + nb*std::sin(angle1);
    color2_v = b*std::cos(angle2) + nb*std::sin(angle2);
    v1 = color1_v*48.0+g*200.0;
    v2 = color2_v*48.0+g*200.0;
    color1 = image::rgb_color(v1[2],v1[1],v1[0]);
    color2 = image::rgb_color(v2[2],v2[1],v2[0]);

    for(int i = 0,pos = 0;i < 3;++i,pos += 3)
    {
        color_m[pos] = color0_v[i];
        color_m[pos+1] = color1_v[i];
        color_m[pos+2] = color2_v[i];
    }
    color_m_inv = color_m;
    color_m_inv.inv();
    //std::cout << color1_v << std::endl;
    //std::cout << color2_v << std::endl;
    //std::cout << v1 << std::endl;
    //std::cout << v2 << std::endl;

}
void wsi::process_mask(void)
{
    image::basic_image<unsigned int,2> labels;
    std::vector<std::vector<unsigned int> > regions;
    image::morphology::connected_component_labeling(map_mask,labels,regions);

    {
        std::vector<int> size_x,size_y;
        image::morphology::get_region_bounding_size(labels,regions,size_x,size_y);

        // remove none-circular region
        for(int i = 0;i < regions.size();++i)
        if(regions[i].size() && (size_x[i] < size_y[i]*0.5 || size_y[i] < size_x[i]*0.5))
            regions[i].clear();
    }

    unsigned int max_size = 1;
    for(int i = 0;i < regions.size();++i)
        if(regions[i].size() > max_size)
            max_size = regions[i].size();
    unsigned int count = 0;
    for(int i = 0;i < regions.size();++i)
        if(regions[i].size() > max_size*0.5)
            ++count;
    is_tma = (count > 10);
    if(is_tma)
    {
        std::vector<unsigned int> id;
        // select regions
        for(int i = 0;i < regions.size();++i)
            if(regions[i].size() > max_size*0.5)
                id.push_back(i);
        // calculate region center
        std::vector<double> x,y;
        for(int i = 0;i < id.size();++i)
            {
                int index = id[i];
                x.push_back(0);
                y.push_back(0);
                for(int j = 0;j < regions[index].size();++j)
                {
                    x.back() += regions[index][j] % map_mask.width();
                    y.back() += int(regions[index][j] / map_mask.width());
                }
                x.back() /= regions[index].size();
                y.back() /= regions[index].size();
            }
        // build up region array
        {
            std::vector<int> ix(id.size()),iy(id.size());
            std::vector<unsigned char> connected(id.size());
            connected[0] = 1;
            float avg_dis = 0;
            for(int total_connected = 1;total_connected < id.size();++total_connected)
            {
                int from = 0,next = 0;
                image::vector<2> dv;
                float dis = std::numeric_limits<float>::max();
                for(int i = 0;i < connected.size();++i)
                    if(connected[i])
                    for(int j = 0;j < connected.size();++j)
                    if(!connected[j])
                    {
                        image::vector<2> d(x[j]-x[i],y[j]-y[i]);
                        if(d.length() < dis)
                        {
                            from = i;
                            next = j;
                            dis = d.length();
                            dv = d;
                        }
                    }
                if(avg_dis == 0)
                    avg_dis = dis;
                else
                {
                    if(dis < avg_dis*1.5)
                    {
                        avg_dis += dis;
                        avg_dis *= 0.5;
                    }
                }
                if(std::abs(dv[0]) > std::abs(dv[1]))
                {
                    ix[next] = ix[from] + std::round(dv[0]/avg_dis);
                    iy[next] = iy[from];
                }
                else
                {
                    ix[next] = ix[from];
                    iy[next] = iy[from] + std::round(dv[1]/avg_dis);
                }
                connected[next] = 1;

            }
            image::minus_constant(ix,*std::min_element(ix.begin(),ix.end()));
            image::minus_constant(iy,*std::min_element(iy.begin(),iy.end()));

            // sort regions
            std::vector<int> value(id.size());
            for(int i = 0;i < id.size();++i)
                value[i] = ix[i] + iy[i]*id.size();
            std::vector<unsigned int> order;
            image::get_sort_index(value,order);
            image::apply_sort_index(id,order);
            image::apply_sort_index(ix,order);
            image::apply_sort_index(iy,order);


            // construct the array map
            int w = *std::max_element(ix.begin(),ix.end()) + 1;
            int h = *std::max_element(iy.begin(),iy.end()) + 1;
            tma_array.resize(image::geometry<2>(w,h));
            tma_results.resize(id.size());
            for(int i = 0;i < id.size();++i)
            {
                int pos = ix[i] + iy[i]*w;
                tma_array[pos] = i+1;
                tma_results[i].resize(tma_feature_count);
            }
        }


        std::fill(map_mask.begin(),map_mask.end(),0);
        tma_map.resize(map_mask.geometry());
        for(int i = 0;i < id.size();++i)
        {
            int index = id[i];
            for(int j = 0;j < regions[index].size();++j)
            {
                tma_map[regions[index][j]] = i+1;
                map_mask[regions[index][j]] = 1;
            }
        }
    }
    else
    {
        image::morphology::smoothing(map_mask);
        image::morphology::smoothing(map_mask);
        image::morphology::smoothing(map_mask);
        image::morphology::defragment(map_mask,0.01);
        image::negate(map_mask);
        image::morphology::defragment(map_mask,0.01);
        image::negate(map_mask);
    }
}
bool wsi::read(openslide_t*& cur_handle,image::color_image& main_image,unsigned int x,unsigned int y,unsigned int level)
{
    openslide_read_region(cur_handle,(uint32_t*)&*main_image.begin(),x,y,level,main_image.width(),main_image.height());
    const char* error = openslide_get_error(cur_handle);
    if(error)
    {
        std::cout << "error ocurred when loading image at (" << x << "," << y << "):" << error << std::endl;
        openslide_close(cur_handle);
        std::cout << "try re-open the WSI" << std::endl;
        cur_handle = openslide_open(file_name.c_str());
        if(!cur_handle)
            std::cout << "re-open WSI failed. Terminating" << std::endl;
        return false;
    }
    if(intensity_normalization)
    {
        if(intensity_map.empty())
        {
            intensity_map.resize(map_image.geometry());
            for(int i = 0;i < map_image.size();++i)
            {
                short value = map_image[i][0];
                value += map_image[i][1];
                value += map_image[i][2];
                intensity_map[i] = value / 3;
            }
            while(intensity_map.width() > 128)
                image::downsampling(intensity_map);

        }
        float r = get_r(level);
        float r2 = (float)intensity_map.width()/(float)dim[0];
        for(image::pixel_index<2> index(main_image.geometry());index.is_valid(main_image.geometry());++index)
        {
            image::vector<2> pos(x,y);
            image::vector<2> shift(index.begin());
            shift *= r;
            pos += shift;
            pos *= r2;
            pos -= 0.5;
            if(intensity_map.geometry().is_valid(pos))
            {
                int s = intensity_norm_value-image::estimate(intensity_map,pos);
                image::vector<3,int> new_c(main_image[index.index()].data);
                new_c += s;
                main_image[index.index()] = new_c.begin();
            }
        }
    }
    if(stain_scale)
    {
        for(int i = 0;i < main_image.size();++i)
        {
            image::vector<3> c(main_image[i].data);
            c.rotate(stain_scale_transform);
            main_image[i] = c.begin();
        }
    }
    return true;
}

void wsi::push_result(std::vector<std::vector<float> >& features)
{
    std::lock_guard<std::mutex> lock(add_data_mutex);
    is_adding_mutex = true;
    result_features.insert(result_features.end(),features.begin(),features.end());
    features.clear();
    is_adding_mutex = false;
}
void wsi::run(unsigned int block_size,
              unsigned int extra_size,
              unsigned int thread_count,
              bool* terminated)
{
    finished = false;
    if(thread_count < 1)
        thread_count = 1;

    ml.init();
    result_features.clear();
    is_adding_mutex = false;

    {
        unsigned int image_size = block_size+extra_size+extra_size;
        std::vector<int> x_list,y_list;
        for(int y = 0;y < dim[1]-extra_size && !(*terminated);y += block_size)
        for(int x = 0;x < dim[0]-extra_size && !(*terminated);x += block_size)
        {
            x_list.push_back(x);
            y_list.push_back(y);
        }

        std::vector<std::vector<std::vector<float> > > features(thread_count);
        std::vector<openslide_t*> handles(thread_count);
        for(int j = 0;j < handles.size();++j)
            handles[j] = openslide_open(file_name.c_str());

        image::time t;
        t.start();
        image::par_for2(x_list.size(),[&](int i,int id)
        {
            if(*terminated)
                return;
            int y = y_list[i];
            int x = x_list[i];
            if(id == 0)
                progress = 100*y/dim[1];

            int map_x1 = std::min<int>((map_mask.width()-1)*(x)/(dim[0]-1),map_mask.width()-1);
            int map_x2 = std::min<int>((map_mask.width()-1)*(x+image_size)/(dim[0]-1),map_mask.width()-1);
            int map_y1 = std::min<int>((map_mask.height()-1)*(y)/(dim[1]-1),map_mask.height()-1);
            int map_y2 = std::min<int>((map_mask.height()-1)*(y+image_size)/(dim[1]-1),map_mask.height()-1);
            image::grayscale_image block_mask;
            image::crop(map_mask,block_mask,image::vector<2>(map_x1,map_y1),
                                 image::vector<2>(map_x2,map_y2));
            if(*std::max_element(block_mask.begin(),block_mask.end()) == 0)
                return;

            unsigned int image_size = block_size + extra_size + extra_size;
            image::color_image I(image::geometry<2>(image_size,image_size));
            if(!read(handles[id],I,x,y,0))
                return;

            image::grayscale_image result;
            ml.recognize(I,result,terminated);
            if(*terminated)
                return;
            std::vector<std::vector<float> > new_features;
            ml.cca(I,result,pixel_size,extra_size,x,y,new_features,terminated,true,color_m_inv);

            if(*terminated)
                return;
            // check whether inside mask
            for(int j = 0;j < new_features.size();++j)
            {
                int map_x = new_features[j][0]*(map_mask.width()-1)/dim[0];
                int map_y = new_features[j][1]*(map_mask.height()-1)/dim[1];
                if(map_mask.geometry().is_valid(map_x,map_y) &&
                        map_mask.at(map_x,map_y))
                    features[id].push_back(std::move(new_features[j]));
            }

            if(!is_adding_mutex && features[id].size() && t.elapsed<std::chrono::seconds>() > 2)
            {
                t.start();
                push_result(features[id]);
            }

        },thread_count);

        for(int j = 0;j < handles.size();++j)
            openslide_close(handles[j]);
        for(int id = 0;id < thread_count;++id)
            push_result(features[id]);
    }

    finished = true;
}
void wsi::save_recognition_result(const char* file_name)
{
    gz_mat_write mat(file_name);
    mat.write("dimension",&*dim.begin(),1,2);
    mat.write("pixel_size",&pixel_size,1,1);
    for(int i = 0;i < feature_count;++i)
    {
        std::vector<float> data(result_features.size());
        for(unsigned int j = 0;j < result_features.size();++j)
            data[j] = result_features[j][i];
        mat.write(feature_list[i],&*data.begin(),1,data.size());
    }
    mat.write("mask",&*map_mask.begin(),map_mask.width(),map_mask.height());
}
void wsi::save_tma_result(const char* file_name,bool label_on_right)
{
    std::ofstream out(file_name);
    for(int m = 0;m < tma_feature_count;++m)
    {
        out << tma_feature_list[m] << std::endl;
        for(int i = 0,index = 0;i < tma_array.height();++i)
        {
            for(int j = 0;j < tma_array.width();++j,++index)
            {
                if(j)
                    out << ", ";
                int id = tma_array[label_on_right ? tma_array.size() -1 - index : index];
                if(id)
                {
                    --id;
                    out << tma_results[id][m];
                }
            }
            out << std::endl;
        }
    }
}

bool wsi::load_recognition_result(const char* file_name)
{
    gz_mat_read mat;
    if(!mat.load_from_file(file_name))
        return false;
    unsigned int rows,cols,cols2;
    const int* dim_ptr = 0;
    const float* pixel_size_ptr = 0;
    if(!mat.read("dimension",rows,cols2,dim_ptr)||
       !mat.read("pixel_size",rows,cols2,pixel_size_ptr))
        return false;
    dim[0] = dim_ptr[0];
    dim[1] = dim_ptr[1];
    pixel_size = *pixel_size_ptr;

    for(int i = 0;i < feature_count;++i)
    {
        const float* ptr = 0;
        if(mat.read(feature_list[i],rows,cols,ptr))
        {
            if(result_features.empty())
            {
                result_features.resize(cols);
                for(int j = 0;j < cols;++j)
                    result_features[j].resize(feature_count);
            }
            for(int j = 0;j < cols;++j)
                result_features[j][i] = ptr[j];
        }
    }
    if(result_features.empty())
        return false;
    const unsigned char* mask = 0;
    if(mat.read("mask",rows,cols,mask))
    {
        map_mask.resize(image::geometry<2>(rows,cols));
        std::copy(mask,mask+map_mask.size(),map_mask.begin());
    }
    process_mask();
    return true;
}

bool wsi::load_text_reco_result(const char* file_name)
{
    std::ifstream in(file_name);
    if(!in)
        return false;
    result_features.clear();
    std::string line;
    while(std::getline(in,line))
    {
        std::vector<float> f;
        std::istringstream values(line);
        std::copy(std::istream_iterator<float>(values),
                  std::istream_iterator<float>(),std::back_inserter(f));
        f.resize(7);
        result_features.push_back(std::move(f));
    }
    return true;
}

void wsi::get_picture(image::color_image& I,int x,int y,unsigned int dim)
{
    I.clear();
    I.resize(image::geometry<2>(dim,dim));
    read(I,x-dim/2,y-dim/2,0);
}

void unmix_color(const image::color_image& I,std::vector<float>& data,const image::matrix<3,3,float>& color_um)
{
    data.resize(I.size()+I.size()+I.size());
    for(int j = 0;j < I.size();++j)
    {
        image::vector<3> rgb(I[j][0],I[j][1],I[j][2]);
        rgb.rotate(color_um);
        data[j] = std::max<float>(0.0f,std::min<float>(1.0f,rgb[0]/570.19f))-0.5;
        data[j+I.size()] = std::max<float>(0.0f,std::min<float>(1.0f,rgb[1]*3.0f/285.1f))-0.5;
        data[j+I.size()+I.size()] = std::max<float>(0.0f,std::min<float>(1.0f,rgb[2]*3.0f/285.1f))-0.5;
    }
}

void wsi::get_picture(std::vector<float>& data,int x,int y,unsigned int dim)
{
    image::color_image I;
    get_picture(I,x,y,dim);
    unmix_color(I,data,color_m_inv);
}

void wsi::get_distribution_image(image::basic_image<float,2>& feature_mapping,
                                 image::basic_image<unsigned char,2>& contour,
                                 float resolution_mm,float band_width_mm,bool feature)
{

    std::vector<std::vector<float> > cur_result_features;
    {
        std::lock_guard<std::mutex> lock(add_data_mutex);
        cur_result_features = result_features;
    }
    image::geometry<2> output_geo;
    output_geo[0] = (float)dim[0]*pixel_size/resolution_mm;
    output_geo[1] = (float)dim[1]*pixel_size/resolution_mm;
    feature_mapping.clear();
    feature_mapping.resize(output_geo);
    contour.clear();
    contour.resize(output_geo);
    if(is_tma)
    {
        for(int i = 0;i < tma_results.size();++i)
            for(int j = 0;j < tma_feature_count;++j)
                tma_results[i][j] = 0;
    }
    image::basic_image<float,2> accumulated_w(output_geo);
    float ratio = pixel_size/resolution_mm;
    float h = band_width_mm/resolution_mm; // band_width in pixels
    int window = std::max<int>(1,h*4.0);  // sampling window in pixels
    float var2 = h*h*2.0;
    for(int index = 0;index < cur_result_features.size();++index)
    {
        image::vector<2> map_pos(cur_result_features[index][target::pos_x],cur_result_features[index][target::pos_y]);
        map_pos *= ratio;
        image::pixel_index<2> map_pos_index(std::round(map_pos[0]),std::round(map_pos[1]),output_geo);
        std::vector<image::pixel_index<2> > map_neighbors;
        image::get_neighbors(map_pos_index,output_geo,window,map_neighbors);
        for(unsigned int j = 0;j < map_neighbors.size();++j)
            {
                image::vector<2> map_neighbor_pos(map_neighbors[j][0],map_neighbors[j][1]);
                map_neighbor_pos -= map_pos;
                float w = std::exp(-map_neighbor_pos.length2()/var2);
                if(feature)
                {
                    feature_mapping[map_neighbors[j].index()] += w*cur_result_features[index][target::span];
                    accumulated_w[map_neighbors[j].index()] += w;
                }
                else
                {
                    feature_mapping[map_neighbors[j].index()] += w;
                }
            }
        if(is_tma)
        {
            image::pixel_index<2> pos(
                            std::round((map_mask.width()-1)*(cur_result_features[index][target::pos_x])/(dim[0]-1)),
                            std::round((map_mask.height()-1)*(cur_result_features[index][target::pos_y])/(dim[1]-1)),
                            map_mask.geometry());
            if(tma_map.geometry().is_valid(pos))
            {
                int id = tma_map[pos.index()];
                if(id)
                {
                    --id;
                    tma_results[id][0] += 1.0;
                    tma_results[id][1] += cur_result_features[index][target::area];
                    tma_results[id][2] += cur_result_features[index][target::intensity];
                }
            }
        }
    }

    if(feature)
    {
        image::lower_threshold(accumulated_w,10.0);
        image::divide(feature_mapping,accumulated_w);
    }
    else
    {
        // output unit in counts per 100 mm ^2
        image::divide_constant(feature_mapping,h*std::sqrt(2.0*3.1415926)*resolution_mm*resolution_mm/100.0);
    }

    if(map_mask.empty())
        return;
    // calculate report

    max_value = *std::max_element(feature_mapping.begin(),feature_mapping.end());
    if(max_value == 0.0)
    {
        mean_value = 0.0;
        q1_value = 0.0;
        q3_value = 0.0;
        return;
    }

    float threshold = max_value*0.01;
    std::vector<float> values;
    values.reserve(feature_mapping.size());
    for(unsigned int index = 0;index < feature_mapping.size();++index)
        if(feature_mapping[index] > threshold)
            values.push_back(feature_mapping[index]);
    if(values.empty())
        return;
    mean_value = std::accumulate(values.begin(),values.end(),0.0)/values.size();
    unsigned int quantile = values.size()/4;
    std::nth_element(values.begin(),values.begin()+quantile,values.end(),std::less<float>());
    q1_value = values[quantile];
    std::nth_element(values.begin(),values.begin()+quantile,values.end(),std::greater<float>());
    q3_value = values[quantile];


    // add contour
    // trim image
    image::scale_nearest(map_mask,contour);
    image::geometry<2> from,to;
    image::bounding_box(contour,from,to,0);
    int border = (to[0]-from[0])*0.1;
    from[0] = std::max<int>(0,from[0]-border);
    from[1] = std::max<int>(0,from[1]-border);
    to[0] = std::min<int>(contour.width()-1,to[0]+border);
    to[1] = std::min<int>(contour.height()-1,to[1]+border);
    if (from[0] < to[0])
    {
        image::crop(feature_mapping,from,to);
        image::crop(contour,from,to);
    }
    image::morphology::edge(contour);

}


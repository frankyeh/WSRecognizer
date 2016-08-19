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

bool wsi::open(const char* file_name)
{
    handle = openslide_open(file_name);
    if(!handle)
        return false;
    {
        int64_t w,h;
        openslide_get_level0_dimensions(handle,&w,&h);
        dim[0] = w;
        dim[1] = h;
        const char* ptr;
        ptr = openslide_get_property_value(handle,"openslide.mpp-x");
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
    {
        float zoom_ratio = 40.0/pixel_size;
        unsigned int level = openslide_get_best_level_for_downsample(handle,zoom_ratio);
        image::color_image I;
        I.resize(dim_at_level[level]);
        openslide_read_region(handle,(uint32_t*)&*I.begin(),0,0,level,I.width(),I.height());
        map_image.resize(image::geometry<2>(dim_at_level[0][0]/zoom_ratio,dim_at_level[0][1]/zoom_ratio));
        image::scale(I,map_image);
    }
    {

        map_mask.resize(map_image.geometry());
        image::grayscale_image gray_image(map_image);
        int threshold = image::segmentation::otsu_threshold(gray_image);
        for(int i = 0;i < gray_image.size();++i)
            map_mask[i] = gray_image[i] > threshold ? 0 : 1;

        image::morphology::smoothing(map_mask);
        image::morphology::smoothing(map_mask);
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
            tma_result.resize(id.size());
            tma_result_pos.resize(id.size());
            for(int i = 0;i < id.size();++i)
            {
                int pos = ix[i] + iy[i]*w;
                tma_result_pos[i] = pos;
                tma_array[pos] = i+1;
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
        image::morphology::defragment(map_mask);
        image::negate(map_mask);
        image::morphology::defragment(map_mask,0.001);
        image::negate(map_mask);
    }
}
void wsi::read(image::color_image& main_image,unsigned int x,unsigned int y,unsigned int level)
{
    std::lock_guard<std::mutex> lock(read_image_mutex);
    openslide_read_region(handle,(uint32_t*)&*main_image.begin(),x,y,level,main_image.width(),main_image.height());
}

void wsi::push_result(std::vector<image::vector<2> >& pos,std::vector<float>& features)
{
    std::lock_guard<std::mutex> lock(add_data_mutex);
    is_adding_mutex = true;
    result_pos.insert(result_pos.end(),pos.begin(),pos.end());
    result_features.insert(result_features.end(),features.begin(),features.end());
    pos.clear();
    features.clear();
    is_adding_mutex = false;
}
void wsi::run(unsigned int block_size,unsigned int extra_size,unsigned int thread_count,bool* terminated)
{
    finished = false;
    if(thread_count < 1)
        thread_count = 1;

    ml.predict(0);// ensure that the features are learned to prevent multithread conflict
    result_pos.clear();
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

        std::vector<std::vector<image::vector<2> > > pos(thread_count);
        std::vector<std::vector<float> > features(thread_count);

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
            read(I,x,y);
            image::grayscale_image result;
            ml.recognize(I,result,terminated);
            if(*terminated)
                return;
            unsigned int pos_size = pos[id].size();
            ml.cca(result,pixel_size,extra_size,pos[id],features[id]);
            image::add_constant(pos[id].begin()+pos_size,pos[id].end(),image::vector<2>(x,y));
            if(!is_adding_mutex && pos[id].size() > 2000)
                push_result(pos[id],features[id]);

        },thread_count);
        for(int id = 0;id < thread_count;++id)
            push_result(pos[id],features[id]);
    }

    finished = true;

}
void wsi::save_recognition_result(const char* file_name)
{
    gz_mat_write mat(file_name);
    mat.write("dimension",&*dim.begin(),1,2);
    mat.write("pixel_size",&pixel_size,1,1);
    std::vector<float> pos_x(result_pos.size()),pos_y(result_pos.size());
    for(unsigned int index = 0;index < result_pos.size();++index)
    {
        pos_x[index] = result_pos[index][0];
        pos_y[index] = result_pos[index][1];
    }
    mat.write("x",&*pos_x.begin(),1,pos_x.size());
    mat.write("y",&*pos_y.begin(),1,pos_x.size());
    mat.write("length",&*result_features.begin(),1,result_features.size());
    mat.write("mask",&*map_mask.begin(),map_mask.width(),map_mask.height());
}
void wsi::save_tma_result(const char* file_name)
{
    std::ofstream out(file_name);
    for(int i = 0,index = 0;i < tma_array.height();++i)
    {
        for(int j = 0;j < tma_array.width();++j,++index)
        {
            if(j)
                out << ", ";
            int id = tma_array[index];
            if(id)
            {
                --id;
                out << tma_result[id];
            }
        }
        out << std::endl;
    }
}

bool wsi::load_recognition_result(const char* file_name)
{
    gz_mat_read mat;
    if(!mat.load_from_file(file_name))
        return false;
    unsigned int rows,cols,cols2;
    const int* dim_ptr = 0;
    const float* x = 0;
    const float* y = 0;
    const float* pixel_size_ptr = 0;
    const float* length = 0;
    if(!mat.read("dimension",rows,cols2,dim_ptr)||
       !mat.read("pixel_size",rows,cols2,pixel_size_ptr) ||
       !mat.read("x",rows,cols,x) ||
       !mat.read("y",rows,cols,y))
        return false;
    mat.read("length",rows,cols,length);
    dim[0] = dim_ptr[0];
    dim[1] = dim_ptr[1];
    pixel_size = *pixel_size_ptr;
    result_pos.resize(cols);
    result_features.resize(cols);
    for(unsigned int index = 0;index < cols;++index)
    {
        result_pos[index][0] = x[index];
        result_pos[index][1] = y[index];
        if(length)
            result_features[index] = length[index];
    }

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
    result_pos.clear();
    result_features.clear();
    std::string line;
    while(std::getline(in,line))
    {
        double x,y,v;
        std::istringstream values(line);
        values >> x >> y >> v;
        result_pos.push_back(image::vector<2>(x,y));
        result_features.push_back(v);
    }
    return true;
}

void wsi::get_distribution_image(image::basic_image<float,2>& feature_mapping,
                                 image::basic_image<unsigned char,2>& contour,
                                 float resolution_mm,float band_width_mm,bool feature,
                                 float min_size,float max_size)
{

    std::vector<image::vector<2> > cur_result_pos;
    std::vector<float> cur_result_features;
    {
        std::lock_guard<std::mutex> lock(add_data_mutex);
        cur_result_pos = result_pos;
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
        std::fill(tma_result.begin(),tma_result.end(),0);
    image::basic_image<float,2> accumulated_w(output_geo);
    float ratio = pixel_size/resolution_mm;
    float h = band_width_mm/resolution_mm; // band_width in pixels
    int window = std::max<int>(1,h*4.0);  // sampling window in pixels
    float var2 = h*h*2.0;
    for(int index = 0;index < cur_result_pos.size();++index)
    {
        if(cur_result_features[index] < min_size ||
           cur_result_features[index] > max_size)
            continue;
        image::vector<2> map_pos(cur_result_pos[index]);
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
                    feature_mapping[map_neighbors[j].index()] += w*cur_result_features[index];
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
                            std::round((map_mask.width()-1)*(cur_result_pos[index][0])/(dim[0]-1)),
                            std::round((map_mask.height()-1)*(cur_result_pos[index][1])/(dim[1]-1)),
                            map_mask.geometry());
            if(tma_map.geometry().is_valid(pos))
            {
                int id = tma_map[pos.index()];
                if(id)
                {
                    --id;
                    ++tma_result[id];
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


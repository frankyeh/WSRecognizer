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
        const unsigned int num_group = 3;
        image::ml::k_means<float,unsigned char> km(num_group);
        km(map_image.begin(),map_image.end(),num_group,map_mask.begin());

        double intensity[num_group] = {0};
        size_t count[num_group] = {0};
        for (size_t index = 0;index < map_mask.size();++index)
        {
            intensity[map_mask[index]] += map_image[index].intensity();
            ++count[map_mask[index]];
        }
        for (size_t index = 0;index < num_group;++index)
            intensity[index] /= count[index];

        size_t background_index = std::max_element(intensity,intensity+num_group) - intensity;

        image::binary(map_mask,std::bind2nd (std::not_equal_to<unsigned char>(), background_index));

        image::morphology::smoothing(map_mask);
        image::morphology::smoothing(map_mask);

        // get the second region
        /*
        if(region_index == 1)
        {
            image::grayscale_image map_mask2(map_mask);
            image::morphology::defragment(map_mask);
            image::negate(map_mask,1);
            image::morphology::defragment(map_mask,0.001);
            image::negate(map_mask,1);
            image::minus(map_mask2.begin(),map_mask2.end(),map_mask.begin());
            map_mask = map_mask2;
        }
        */
        image::morphology::defragment(map_mask);
        image::negate(map_mask,1);
        image::morphology::defragment(map_mask,0.001);
        image::negate(map_mask,1);
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
    return true;
}
void wsi::read(image::color_image& main_image,unsigned int x,unsigned int y,unsigned int level)
{
    boost::mutex::scoped_lock lock(read_image_mutex);
    openslide_read_region(handle,(uint32_t*)&*main_image.begin(),x,y,level,main_image.width(),main_image.height());
}


void wsi::run_block(unsigned char* running,unsigned int x,unsigned int y,unsigned int block_size,unsigned int extra_size,
                    bool* terminated)
{
    unsigned int image_size = block_size + extra_size + extra_size;
    image::color_image I(image::geometry<2>(image_size,image_size));
    read(I,x,y);
    image::grayscale_image result;
    ml.recognize(I,result,terminated);
    if(*terminated)
        return;
    std::vector<image::vector<2> > pos;
    std::vector<float> features;
    ml.cca(result,pixel_size,extra_size,pos,features);
    image::add_constant(pos,image::vector<2>(x,y));
    {
        boost::mutex::scoped_lock lock(read_image_mutex);
        if(result_pos.empty())
        {
            result_pos.swap(pos);
            result_features.swap(features);
        }
        else
        {
            result_pos.insert(result_pos.end(),pos.begin(),pos.end());
            result_features.insert(result_features.end(),features.begin(),features.end());
        }
    }
    if(running)
        *running = 0;
}

void wsi::run(unsigned int block_size,unsigned int extra_size,unsigned int thread_count,bool* terminated)
{
    finished = false;
    if(thread_count < 1)
        thread_count = 1;

    ml.predict(0);// ensure that the features are learned to prevent multithread conflict
    result_pos.clear();
    result_features.clear();

    {
        unsigned int image_size = block_size+extra_size+extra_size;
        std::vector<unsigned char> thread_running(thread_count);
        std::vector<boost::thread*> threads(thread_count);
        for(int y = 0;y < dim[1]-extra_size && !(*terminated);y += block_size)
        for(int x = 0;x < dim[0]-extra_size && !(*terminated);x += block_size)
        {
            progress = 100*y/dim[1];
            int map_x1 = std::min<int>((map_mask.width()-1)*(x)/(dim[0]-1),map_mask.width()-1);
            int map_x2 = std::min<int>((map_mask.width()-1)*(x+image_size)/(dim[0]-1),map_mask.width()-1);
            int map_y1 = std::min<int>((map_mask.height()-1)*(y)/(dim[1]-1),map_mask.height()-1);
            int map_y2 = std::min<int>((map_mask.height()-1)*(y+image_size)/(dim[1]-1),map_mask.height()-1);
            image::grayscale_image block_mask;
            image::crop(map_mask,block_mask,image::vector<2>(map_x1,map_y1),
                                 image::vector<2>(map_x2,map_y2));
            if(*std::max_element(block_mask.begin(),block_mask.end()) == 0)
                continue;
            bool found = false;
            do
             for(unsigned int index = 0;index < threads.size();++index)
                if(!thread_running[index])
                {
                    delete threads[index];
                    thread_running[index] = 1;
                    threads[index] = new boost::thread(&wsi::run_block,this,
                        &thread_running[index],x,y,block_size,extra_size,terminated);
                    found = true;
                    break;
                }
                while(!found && !(*terminated));
        }
        for(unsigned int index = 0;index < threads.size();++index)
        {
            if(threads[index])
            {
                threads[index]->join();
                delete threads[index];
            }
        }
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
    boost::mutex::scoped_lock lock(read_image_mutex);
    image::geometry<2> output_geo;
    output_geo[0] = (float)dim[0]*pixel_size/resolution_mm;
    output_geo[1] = (float)dim[1]*pixel_size/resolution_mm;
    feature_mapping.clear();
    feature_mapping.resize(output_geo);
    contour.clear();
    contour.resize(output_geo);
    image::basic_image<float,2> accumulated_w(output_geo);
    float ratio = pixel_size/resolution_mm;
    float h = band_width_mm/resolution_mm; // band_width in pixels
    int window = std::max<int>(1,h*4.0);  // sampling window in pixels
    float var2 = h*h*2.0;
    for(unsigned int index = 0;index < result_pos.size();++index)
    {
        image::vector<2> map_pos(result_pos[index]);
        map_pos *= ratio;
        image::pixel_index<2> map_pos_index(
                    std::floor(map_pos[0]+0.5),std::floor(map_pos[1]+0.5),output_geo);
        std::vector<image::pixel_index<2> > map_neighbors;
        image::get_neighbors(map_pos_index,output_geo,window,map_neighbors);
        for(unsigned int j = 0;j < map_neighbors.size();++j)
            {
                image::vector<2> map_neighbor_pos(map_neighbors[j][0],map_neighbors[j][1]);
                map_neighbor_pos -= map_pos;
                float w = std::exp(-map_neighbor_pos.length2()/var2);
                if(feature)
                {
                    if(result_features[index] >= min_size &&
                       result_features[index] <= max_size)
                    {
                        feature_mapping[map_neighbors[j].index()] += w*result_features[index];
                        accumulated_w[map_neighbors[j].index()] += w;
                    }
                }
                else
                {
                    feature_mapping[map_neighbors[j].index()] += w;
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


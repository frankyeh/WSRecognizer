#ifndef WSI_HPP
#define WSI_HPP
#include "image/image.hpp"
#include <string>
#include "train_model.hpp"
typedef struct _openslide openslide_t;
const unsigned int tma_feature_count = 3;
const char tma_feature_list[tma_feature_count][10] = {"count","area","intensity"};

class wsi
{
public:
    openslide_t* handle;
    std::string file_name;
    image::geometry<2> dim;
    float pixel_size; // in micron;
    unsigned int level;
public:
    bool intensity_normalization = false;
    float intensity_norm_value = 0;
public:
    bool is_tma;
    image::basic_image<int,2> tma_map;
    image::basic_image<int,2> tma_array;
public:
    std::vector<std::vector<float> > tma_results;

public:
    image::color_image map_image;
    image::grayscale_image intensity_map;
    image::grayscale_image map_mask;
public:
    std::vector<std::string> property_name;
    std::vector<std::string> property_value;
public:
    std::vector<std::string> associated_image_name;
    std::vector<image::basic_image<image::rgb_color,2> > associated_image;
public:
    wsi();
    ~wsi();
    bool open(const char* file_name);
    void process_mask(void);
public:
    bool read(openslide_t*& cur_handle,image::color_image& main_image,unsigned int x,unsigned int y,unsigned int level);
    bool read(image::color_image& main_image,unsigned int x,unsigned int y,unsigned int level)
    {
        return read(handle,main_image,x,y,level);
    }
private:
    std::vector<image::geometry<2> > dim_at_level;
    std::vector<double> r_at_level;
public:
    double get_r(int level) const
    {
        if(level < 0)
            return get_r(level+1)*0.5;
        return r_at_level[level];
    }
    int get_height(int level) const
    {
        if(level < 0)
            return get_height(level+1)*2;
        return dim_at_level[level][1];
    }
    int get_width(int level) const
    {
        if(level < 0)
            return get_width(level+1)*2;
        return dim_at_level[level][0];
    }
    float get_pixel_size(int level) const
    {
        return pixel_size*get_r(level);
    }

    template<typename value_type>
    void map_coordinate_to_level(value_type& x,value_type& y,int level)
    {
        x = (float)x/(float)map_image.width()*(float)dim_at_level[level][0];
        y = (float)y/(float)map_image.height()*(float)dim_at_level[level][1];
    }
    template<typename value_type>
    void level_coordinate_to_map(value_type& x,value_type& y,int level)
    {
        x = (float)x*(float)map_image.width()/(float)dim_at_level[level][0];
        y = (float)y*(float)map_image.height()/(float)dim_at_level[level][1];
    }

public:
    std::mutex add_data_mutex;
    bool is_adding_mutex;
    train_model ml;
    std::vector<std::vector<float> > result_features;
    unsigned int progress;
    bool finished;
    void push_result(std::vector<std::vector<float> >& features);
    void run(unsigned int block_size,unsigned int extra_size,
             unsigned int thread_count,bool* terminated);
    void save_recognition_result(const char* file_name);
    void save_tma_result(const char* file_name,bool label_on_right);
    bool load_recognition_result(const char* file_name);
    bool load_text_reco_result(const char* file_name);
    void get_distribution_image(image::basic_image<float,2>& feature_mapping,
                                image::basic_image<unsigned char,2>& contour,
                                float resolution_mm,float band_width_mm,bool feature);
    void get_picture(image::color_image& I,int x,int y,unsigned int dim);
    void get_picture(std::vector<float>& I,int x,int y,unsigned int dim);

public: // color profile
    image::vector<3> color0_v,color1_v,color2_v;
    image::rgb_color color1,color2;
    double color1_count,color2_count;
    void read_profile(const image::color_image& I);
public:
    bool stain_scale = false;
    image::matrix<3,3,float> stain_scale_transform;
    void set_stain_scale(float stain1_scale,float stain2_scale);
public: // report
    float mean_value;
    float max_value;
    float q1_value,q3_value;
public:
    static bool can_open(const char* file_name);
};

#endif // WSI_HPP

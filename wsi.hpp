#ifndef WSI_HPP
#define WSI_HPP
#include <boost/thread.hpp>
#include "image/image.hpp"
#include <string>
#include "train_model.hpp"
typedef struct _openslide openslide_t;

class wsi
{
public:
    openslide_t* handle;
    image::geometry<2> dim;
    float pixel_size; // in micron;
    std::vector<image::geometry<2> > dim_at_level;
    std::vector<double> r_at_level;
    unsigned int level;
public:
    image::color_image map_image;
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
public:
    boost::mutex read_image_mutex;
    void read(image::color_image& main_image,unsigned int x,unsigned int y);
private:
    void run_block(unsigned char* running,unsigned int x,unsigned int y,unsigned int block_size,unsigned int extra_size,
                   bool* terminated);
public:
    boost::mutex add_data_mutex;
    train_model ml;
    std::vector<image::vector<2> > result_pos;
    std::vector<float> result_features;
    unsigned int progress;
    bool finished;
    void run(unsigned int block_size,unsigned int extra_size,
             unsigned int thread_count,bool* terminated);
    void save_recognition_result(const char* file_name);
    bool load_recognition_result(const char* file_name);
    bool load_text_reco_result(const char* file_name);
    void get_distribution_image(image::basic_image<float,2>& feature_mapping,
                                image::basic_image<unsigned char,2>& contour,
                                float resolution_mm,float band_width_mm,bool feature,
                                float min_size,float max_size);
    void get_result_txt(std::vector<std::string>& name,
                        std::vector<float>& value);
public: // report
    float mean_value;
    float max_value;
    float q1_value,q3_value;
public:
    static bool can_open(const char* file_name);
};

#endif // WSI_HPP

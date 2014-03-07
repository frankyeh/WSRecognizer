#ifndef TRAIN_MODEL_HPP
#define TRAIN_MODEL_HPP
#include "image/image.hpp"

class train_model
{
private:
    //typedef image::ml::nearest_neighbor<double,unsigned char,image::ml::weight_function_average> model_type;
    typedef image::ml::svm<double,unsigned char> model_type;
    image::ml::training_data<double,unsigned char> data;
    std::auto_ptr<model_type> ml_model;
    std::vector<unsigned char> solution_space;
public:// for showing the rgb distribution
    image::color_image classifier_map;
    double r[9];
public://parameters
    unsigned int smoothing;
private:
public:
    train_model(void);
    const train_model& operator=(const train_model& rhs)
    {
        clear();
        solution_space = rhs.solution_space;
        data = rhs.data;
        classifier_map = rhs.classifier_map;
        smoothing = rhs.smoothing;
        std::copy(rhs.r,rhs.r+9,r);
        return *this;
    }
    void update_classifier_map(void);
    bool is_trained(void) const;
    void clear(void);
    void add_data(const image::color_image& I,bool background);
    void add_data(image::rgb_color color,bool background);

public:
    unsigned char predict(image::rgb_color value);
    void recognize(const image::color_image& I,image::grayscale_image& result,bool* terminated = 0);
    void cca(const image::grayscale_image& result,float pixel_size,
             unsigned int border,std::vector<image::vector<2> >& pos,std::vector<float>& features);
    bool load_from_file(const char* file_name);

    void save_to_file(const char* file_name);
public:
    static void get_position(const image::vector<3,double>& pos,
                                          const image::vector<2,double>& x_dir,
                                          const image::vector<2,double>& y_dir,
                                          const image::vector<2,double>& z_dir,
                                          double size,
                                          int& x,int& y);

};

#endif // TRAIN_MODEL_HPP

#ifndef TRAIN_MODEL_HPP
#define TRAIN_MODEL_HPP
#include "image/image.hpp"

class train_model
{
private:
    typedef image::ml::nearest_neighbor<double,unsigned char,image::ml::weight_function_average> model_type;
    std::auto_ptr<model_type> nearest_neighbor;
    std::vector<unsigned char> solution_space;
    double atts[3];
public:
    image::ml::training_data<double,unsigned char> data;
    image::color_image classifier_map;
    double r[9];
private:
public:
    train_model(void);
    void update_classifier_map(void);
    void clear(void);
    void add_data(const image::color_image& I,bool background);
    bool is_empty(void) const{return data.features.empty();}
public:
    unsigned char predict(image::rgb_color value);
    void recognize(const image::color_image& I,image::grayscale_image& result,bool* terminated = 0);
    void cca(const image::grayscale_image& result,
             unsigned int border,unsigned char feature_type,
             std::vector<image::vector<2> >& pos,std::vector<float>& features);
public:
    static void get_position(const image::vector<3,double>& pos,
                                          const image::vector<2,double>& x_dir,
                                          const image::vector<2,double>& y_dir,
                                          const image::vector<2,double>& z_dir,
                                          double size,
                                          int& x,int& y);

};

#endif // TRAIN_MODEL_HPP

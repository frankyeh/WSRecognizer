#ifndef TRAIN_MODEL_HPP
#define TRAIN_MODEL_HPP
#include "tipl/tipl.hpp"

//feature list
const unsigned int feature_count = 5;
const char feature_list[feature_count][35] = {"x","y","span (microns)","area (microns^2)","intensity"};
enum target {pos_x=0, pos_y = 1,span = 2, area = 3, intensity = 4} ;


class train_model
{
private:
    typedef tipl::ml::svm<float,unsigned char> model_type;
    tipl::ml::training_data<float,unsigned char> data;
    std::shared_ptr<model_type> ml_model;
    std::vector<unsigned char> solution_space;
public:
    tipl::ml::network nn;
public:// for showing the rgb distribution
    tipl::color_image classifier_map;
    std::vector<float> r;
    void update_classifier_map(size_t image_size)
    {
        classifier_map.clear();
        if (data.features.size())
        {
            classifier_map.resize(tipl::geometry<2>(image_size,image_size));
            std::fill(classifier_map.begin(),classifier_map.end(),tipl::rgb(255,255,255));
            tipl::vector<2,double> x_dir,y_dir,z_dir;
            x_dir[0] = r[0];
            x_dir[1] = r[3];
            y_dir[0] = r[1];
            y_dir[1] = r[4];
            z_dir[0] = r[2];
            z_dir[1] = r[5];
            for (size_t index = 0;index < data.features.size();++index)
            {
                tipl::vector<3,double> pos;
                std::copy(data.features[index].begin(),data.features[index].end(),pos.begin());
                int x,y;
                get_position(pos,x_dir,y_dir,z_dir,image_size,x,y);
                if (!classifier_map.geometry().is_valid(x,y))
                    continue;
                size_t pindex = x + y*classifier_map.width();
                if (data.classification[index])
                    {
                        classifier_map[pindex].g >>=1;
                        classifier_map[pindex].b >>=1;
                    }
                    else
                    {
                        classifier_map[pindex].r >>=1;
                        classifier_map[pindex].g >>=1;
                    }
            }
        }
    }
public:
    static void get_position(const tipl::vector<3,double>& pos,
                      const tipl::vector<2,double>& x_dir,
                      const tipl::vector<2,double>& y_dir,
                      const tipl::vector<2,double>& z_dir,
                      double size,
                      int& x,int& y)
    {
        tipl::vector<2,double> present_location(1,1);
        present_location += x_dir * (pos[0]-0.5);
        present_location += y_dir * (pos[1]-0.5);
        present_location += z_dir * (pos[2]-0.5);
        //present_location[0] += pos[0]-0.5;
        //present_location[1] += pos[2]-0.5;

        present_location *= size;
        present_location /= 2.0;
        x = present_location[0];//+(rand() & 3) - 2.0;
        y = present_location[1];//+(rand() & 3) - 2.0;
    }
    bool has_data(void) const{return !data.features.empty();}
    bool is_trained(void) const
    {
        bool has_foreground = false;
        bool has_background = false;
        for(unsigned int index = 0;index < data.classification.size();++index)
            if(data.classification[index])
                has_foreground = true;
            else
                has_background = true;
        return has_foreground && has_background;
    }

    void init(void)
    {
        if(!ml_model.get())
        {
            ml_model.reset(new model_type);
            ml_model->learn(data.features.begin(),data.features.end(),3,data.classification.begin());
            solution_space.clear();
            solution_space.resize(256*256*256);
            std::fill(solution_space.begin(),solution_space.end(),0);
        }
    }

    unsigned char predict(tipl::rgb value)
    {
        value.a = 0;
        unsigned char result = solution_space[value.color];
        if (!result)
        {
            float atts[3];
            atts[0] = ((float)value.r/255.0f);
            atts[1] = ((float)value.g/255.0f);
            atts[2] = ((float)value.b/255.0f);
            result = solution_space[value.color] = ml_model->predict(atts)+1;
        }
        return result-1;
    }
public://parameters
    std::vector<float> param;
    bool fulfill_param(const std::vector<float> features) const;
private:
public:
    train_model(void);
    const train_model& operator=(const train_model& rhs)
    {
        clear();
        data.features = rhs.data.features;
        data.classification = rhs.data.classification;
        param = rhs.param;
        ml_model.reset();

        return *this;
    }
    void clear(void);
    void add_data(tipl::rgb color,bool background);

public:
    void recognize(const tipl::color_image& I,tipl::grayscale_image& result,bool* terminated = 0);
public:
    void cca(const tipl::color_image& I,const tipl::grayscale_image& result,float pixel_size,
             unsigned int border,int x,int y,std::vector<std::vector<float> >& features,bool* terminated,
             bool apply_ml,const tipl::matrix<3,3,float>& color_unmix);
    bool load_from_file(const char* file_name);

    void save_to_file(const char* file_name);


};

#endif // TRAIN_MODEL_HPP

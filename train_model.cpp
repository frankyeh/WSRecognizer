#include "train_model.hpp"
#include "libs/gzip_interface.hpp"
void train_model::get_position(const image::vector<3,double>& pos,
                  const image::vector<2,double>& x_dir,
                  const image::vector<2,double>& y_dir,
                  const image::vector<2,double>& z_dir,
                  double size,
                  int& x,int& y)
{
    image::vector<2,double> present_location(1,1);
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
train_model::train_model(void):solution_space(256*256*256)
{
    std::fill(r,r+9,0.0);
    r[0] = r[4] = r[8] = 1.0;
}
bool train_model::is_trained(void) const
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
unsigned char train_model::predict(image::rgb_color value)
{
    if(!ml_model.get())
    {
        ml_model.reset(new model_type);
        ml_model->learn(data.features.begin(),data.features.end(),3,data.classification.begin());
        std::fill(solution_space.begin(),solution_space.end(),0);
    }
    unsigned char result = solution_space[value.color];
    if (!result)
    {
        double atts[3];
        atts[0] = ((double)value.r/255.0);
        atts[1] = ((double)value.g/255.0);
        atts[2] = ((double)value.b/255.0);
        result = solution_space[value.color] = ml_model->predict(atts)+1;
    }
    return result-1;
}

void train_model::recognize(const image::color_image& I,image::grayscale_image& result,bool* terminated)
{
    result.resize(I.geometry());
    for (unsigned int index = 0;index < result.size();++index)
    {
        image::rgb_color value = I[index];
        value[3] = 0;
        result[index] = predict(value) ? 255 : 0;
        if(terminated && *terminated)
            return;
    }
    if(smoothing)
        image::morphology::recursive_smoothing(result,smoothing);
}
void train_model::cca(const image::grayscale_image& result,
                      float pixel_size,
                      unsigned int border,
                      std::vector<image::vector<2> >& pos,
                      std::vector<float>& features)
{
    image::basic_image<unsigned int,2> labels;
    std::vector<std::vector<unsigned int> > regions;
    image::morphology::connected_component_labeling(result,labels,regions);
    unsigned int upper_border = result.width()-border;
    std::vector<image::vector<2,float> > center_of_mass(regions.size());
    std::vector<image::vector<2,int> > max_pos(regions.size()),min_pos(regions.size());
    std::fill(min_pos.begin(),min_pos.end(),image::vector<2,float>(result.geometry()[0],result.geometry()[1]));
    for (image::pixel_index<2> index;index.is_valid(result.geometry());index.next(result.geometry()))
    {
        size_t region_id = labels[index.index()]-1;
        if (!result[index.index()] || regions[region_id].empty())
            continue;
        center_of_mass[region_id] += image::vector<2,float>(index);
        max_pos[region_id][0] = std::max<int>(index[0],max_pos[region_id][0]);
        max_pos[region_id][1] = std::max<int>(index[1],max_pos[region_id][1]);
        min_pos[region_id][0] = std::min<int>(index[0],min_pos[region_id][0]);
        min_pos[region_id][1] = std::min<int>(index[1],min_pos[region_id][1]);
    }

    for(size_t index = 0;index < regions.size();++index)
    {
        if(regions[index].empty())
            continue;
        center_of_mass[index] /= regions[index].size();
        if(center_of_mass[index][0] < border || center_of_mass[index][0] >= upper_border ||
           center_of_mass[index][1] < border || center_of_mass[index][1] >= upper_border)
            continue;
        float feature_size = (max_pos[index][0]-min_pos[index][0]+(max_pos[index][1]-min_pos[index][1]))/2.0;
        if(border && feature_size > border)
            continue;
        feature_size *= pixel_size; // now in micron
        pos.push_back(center_of_mass[index]);
        features.push_back(feature_size);
    }
}


void train_model::clear()
{
    std::fill(solution_space.begin(),solution_space.end(),0);
    data.classification.clear();
    data.features.clear();
    classifier_map.clear();
    ml_model.reset(0);
}
void train_model::update_classifier_map(void)
{
    classifier_map.clear();
    if (data.features.size())
    {
        size_t image_size = 200;
        classifier_map.resize(image::geometry<2>(image_size,image_size));
        std::fill(classifier_map.begin(),classifier_map.end(),image::rgb_color(255,255,255));
        image::vector<2,double> x_dir,y_dir,z_dir;
        x_dir[0] = r[0];
        x_dir[1] = r[3];
        y_dir[0] = r[1];
        y_dir[1] = r[4];
        z_dir[0] = r[2];
        z_dir[1] = r[5];
        for (size_t index = 0;index < data.features.size();++index)
        {
            image::vector<3,double> pos;
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

bool train_model::load_from_file(const char* file_name)
{
    gz_mat_read mat;
    if(!mat.load_from_file(file_name))
        return false;
    unsigned int row,col;
    const unsigned int* color = 0;
    const unsigned char* label = 0;
    if(!mat.read("color",row,col,color) ||
       !mat.read("label",row,col,label))
        return false;
    image::ml::training_data<double,unsigned char> new_data;
    new_data.features.resize(col);
    new_data.classification.resize(col);
    for(unsigned int index = 0;index < new_data.features.size();++index)
    {
        image::rgb_color cur_color;
        cur_color.color = color[index];
        new_data.features[index].resize(3);
        new_data.features[index][0] = ((double)cur_color.r)/255.0;
        new_data.features[index][1] = ((double)cur_color.g)/255.0;
        new_data.features[index][2] = ((double)cur_color.b)/255.0;
        new_data.classification[index] = label[index];
    }
    const unsigned int* param = 0;
    if(!mat.read("param",row,col,param))
        return false;
    clear();
    data.features.swap(new_data.features);
    data.classification.swap(new_data.classification);
    smoothing = param[0];
    update_classifier_map();
    return true;
}

void train_model::save_to_file(const char* file_name)
{
    if(data.features.empty())
        return;
    gz_mat_write mat(file_name);

    std::vector<unsigned int> color(data.features.size());
    std::vector<unsigned char> label(data.classification.size());
    for(unsigned int index = 0;index < color.size();++index)
    {
        color[index] = image::rgb_color(data.features[index][0]*255.0,
                                        data.features[index][1]*255.0,
                                        data.features[index][2]*255.0);
        label[index] = data.classification[index];
    }
    std::vector<unsigned int> param(10);
    param[0] = smoothing;
    mat.write("color",&*color.begin(),1,color.size());
    mat.write("label",&*label.begin(),1,label.size());
    mat.write("param",&*param.begin(),1,param.size());
}

void train_model::add_data(const image::color_image& I,bool background)
{
    for (size_t index = 0;index < I.size();++index)
    {
        if (I[index] == image::rgb_color(255,255,255))
            continue;
        add_data(I[index],background);
    }
    update_classifier_map();
    ml_model.reset(0);
}

void train_model::add_data(image::rgb_color color,bool background)
{
    double atts[3];
    atts[0] = ((double)color.r/255.0);
    atts[1] = ((double)color.g/255.0);
    atts[2] = ((double)color.b/255.0);
    data.features.resize(data.features.size()+1);
    data.features.back().resize(3);
    std::copy(atts,atts+3,data.features.back().begin());
    data.classification.push_back(background ? 0 : 1);
    ml_model.reset(0);
}

#include "train_model.hpp"
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
unsigned char train_model::predict(image::rgb_color value)
{
    if(!nearest_neighbor.get())
    {
        nearest_neighbor.reset(new model_type);
        nearest_neighbor->learn(data.features.begin(),data.features.end(),3,data.classification.begin());
    }
    unsigned char result = solution_space[value.color];
    if (!result)
    {
        atts[0] = ((double)value.r/255.0);
        atts[1] = ((double)value.g/255.0);
        atts[2] = ((double)value.b/255.0);
        result = solution_space[value.color] = nearest_neighbor->predict(atts)+1;
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
    for(unsigned int iter = 0;iter < 4;++iter)
        image::morphology::smoothing(result);
}
void train_model::cca(const image::grayscale_image& result,
                      unsigned int border,unsigned char feature_type,
         std::vector<image::vector<2> >& pos,std::vector<float>& features)
{
    image::basic_image<unsigned int,2> labels;
    std::vector<std::vector<unsigned int> > regions;
    image::morphology::connected_component_labeling(result,labels,regions);
    unsigned int upper_border = result.width()-border;
    switch(feature_type)
    {
    case 0:// area
        break;
    case 1:// size
        {
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
                pos.push_back(center_of_mass[index]);
                features.push_back((max_pos[index][0]-min_pos[index][0]+(max_pos[index][1]-min_pos[index][1]))/2.0);
            }
        }
        break;
    }
}


void train_model::clear()
{
    std::fill(solution_space.begin(),solution_space.end(),0);
    data.classification.clear();
    data.features.clear();
    update_classifier_map();
    nearest_neighbor.reset(0);
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

void train_model::add_data(const image::color_image& I,bool background)
{
    for (size_t index = 0;index < I.size();++index)
    {
        image::rgb_color value = I[index];
        if (value == image::rgb_color(255,255,255))
            continue;
        double atts[3];
        atts[0] = ((double)value.r/255.0);
        atts[1] = ((double)value.g/255.0);
        atts[2] = ((double)value.b/255.0);
        data.features.resize(data.features.size()+1);
        data.features.back().resize(3);
        std::copy(atts,atts+3,data.features.back().begin());
        data.classification.push_back(background ? 0 : 1);
    }

    update_classifier_map();
}

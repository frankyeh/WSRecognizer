#include "train_model.hpp"
#include "libs/gzip_interface.hpp"

train_model::train_model(void):r(9),param(10)
{
    r[0] = r[4] = r[8] = 1.0;
}

void train_model::recognize(const image::color_image& I,image::grayscale_image& result,bool* terminated)
{
    init();
    result.resize(I.geometry());
    for (unsigned int index = 0;index < result.size();++index)
    {
        result[index] = predict(I[index]) ? 255:0;
        if(terminated && *terminated)
            return;
    }
    if(param[0] != 0.0)
        image::morphology::recursive_smoothing(result,param[0]);
}
void train_model::cca(const image::color_image& I,
                      const image::grayscale_image& result,
                      float pixel_size,
                      unsigned int border,
                      int x,
                      int y,
                      std::vector<std::vector<float> >& features)
{

    image::basic_image<unsigned int,2> labels;
    std::vector<std::vector<unsigned int> > regions;
    image::morphology::connected_component_labeling(result,labels,regions);
    unsigned int upper_border = result.width()-border;

    std::vector<image::vector<2,float> > center_of_mass;
    std::vector<int> size_x,size_y;
    image::morphology::get_region_bounding_size(labels,regions,size_x,size_y);
    image::morphology::get_region_center(labels,regions,center_of_mass);

    int fov_range = std::min<int>(20,std::max<int>(4,param[2]));

    for(size_t index = 0;index < regions.size();++index)
    if(!regions[index].empty())
    {
        if(center_of_mass[index][0] < border || center_of_mass[index][0] >= upper_border ||
           center_of_mass[index][1] < border || center_of_mass[index][1] >= upper_border)
            continue;
        float span = std::max(size_x[index],size_y[index]);
        if(border && span > border)
            continue;


        std::vector<image::pixel_index<2> > neighbors;
        image::get_neighbors(image::pixel_index<2>(center_of_mass[index][0],center_of_mass[index][1],
                result.geometry()),result.geometry(),fov_range,neighbors);


        float sum_in = 0.0f,sum_out = 0.0f;
        unsigned int count_in = 0,count_out = 0;
        for(int i = 0;i < neighbors.size();++i)
        {
            int pos = neighbors[i].index();
            if(result[pos])
            {
                sum_in += I[pos].r;
                sum_in += I[pos].g;
                sum_in += I[pos].b;
                ++count_in;
            }
        else
            {
                sum_out += I[pos].r;
                sum_out += I[pos].g;
                sum_out += I[pos].b;
                ++count_out;
            }
        }
        if(count_in)
            sum_in /= count_in;
        if(count_out)
            sum_out /= count_out;
        float intensity = (sum_out - sum_in)/255.0;
        std::vector<float> f;
        // feature list 0:x 1:y 2:span 3:area 4:shape 5:intensity gradient
        f.push_back(center_of_mass[index][0]+x);
        f.push_back(center_of_mass[index][1]+y);
        f.push_back(span*pixel_size);
        f.push_back(regions[index].size()*pixel_size*pixel_size);
        f.push_back(regions[index].size()/span/span);
        f.push_back(intensity);
        if(!fulfill_param(f))
            continue;
        if(!nn.empty())
        {
            image::color_image I2;
            int dim= nn.get_input_dim()[0];
            int shift = dim >> 1;
            image::vector<2,int> from(center_of_mass[index][0]-shift,center_of_mass[index][1]-shift);
            image::vector<2,int> to(from);
            to += dim;
            if(from[0] < 0 || from[1] < 0 || to[0] >= I.width() || to[1] >= I.height())
                ;
            else
            {
                image::crop(I,I2,from,to);
                std::vector<float> data(dim*dim*3);
                for(int i = 0,index = 0;i < 3;++i)
                    for(int j = 0;j < I2.size();++j,++index)
                        data[index] = ((float)I2[j][2-i]/255.0f-0.5f)*2.0f;

                nn.predict(data);
                float score = data[1]-data[0];
                if(score < 0.0)
                    continue;
                f.push_back(score);
            }
        }
        else
            f.push_back(0.0f);
        features.push_back(std::move(f));
    }
}

bool train_model::fulfill_param(const std::vector<float> features) const
{
    // feature list 0:x 1:y 2:span 3:area 4:shape 5:intensity gradient
    // check span
    if(param[1] != 0.0f || param[2] != 0.0f)
    {
        if(features[2] < param[1] || features[2] > param[2])
            return false;
    }
    // check area
    if(param[3] != 0.0f || param[4] != 0.0f)
    {
        if(features[3] < param[3] || features[3] > param[4])
            return false;
    }
    // check shape
    if(param[5] != 0.0f || param[6] != 0.0f)
    {
        if(features[4] < param[5] || features[4] > param[6])
            return false;
    }
    // check intensity gradient
    if(param[7] != 0.0f || param[8] != 0.0f)
    {
        if(features[5] < param[7] || features[5] > param[8])
            return false;
    }
    return true;
}

void train_model::clear()
{
    data.features.clear();
    data.classification.clear();
    ml_model.reset();
    solution_space.clear();
    classifier_map.clear();
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
    int sample_size = col;
    const float* param_ptr = 0;
    if(!mat.read("param",row,col,param_ptr))
        return false;
    clear();
    data.features.resize(sample_size);
    data.classification.resize(sample_size);
    for(int i = 0;i < sample_size;++i)
    {
        image::rgb_color cur_color;
        cur_color.color = color[i];
        data.features[i].resize(3);
        data.features[i][0] = ((float)cur_color.r)/255.0f;
        data.features[i][1] = ((float)cur_color.g)/255.0f;
        data.features[i][2] = ((float)cur_color.b)/255.0f;
        data.classification[i] = label[i];
    }
    ml_model.reset();
    init();
    int param_size = param.size();
    param.clear();
    std::copy(param_ptr,param_ptr+row*col,std::back_inserter(param));
    if(param.size() < param_size)
        param.resize(param_size);
    return true;
}

void train_model::save_to_file(const char* file_name)
{
    if(data.features.empty())
        return;
    gz_mat_write mat(file_name);
    unsigned int sample_size = data.features.size();
    std::vector<unsigned int> color(sample_size);
    std::vector<unsigned char> label(sample_size);
    for(int i = 0;i < data.features.size();++i)
    {
        color[i] = image::rgb_color(data.features[i][0]*255.0,
                                        data.features[i][1]*255.0,
                                        data.features[i][2]*255.0);
        label[i] = data.classification[i];
    }
    mat.write("color",&*color.begin(),1,sample_size);
    mat.write("label",&*label.begin(),1,sample_size);
    mat.write("param",&*param.begin(),1,param.size());
}

void train_model::add_data(image::rgb_color color,bool background)
{
    float atts[3];
    atts[0] = ((float)color.r/255.0f);
    atts[1] = ((float)color.g/255.0f);
    atts[2] = ((float)color.b/255.0f);
    data.features.resize(data.features.size()+1);
    data.features.back().resize(3);
    std::copy(atts,atts+3,data.features.back().begin());
    data.classification.push_back(background ? 0 : 1);
    if(ml_model.get())
        ml_model.reset();
}

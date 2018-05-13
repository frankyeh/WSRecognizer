#include <QImage>
#include <QFileInfo>
#include <iostream>
#include <iterator>
#include <string>
#include "tipl/tipl.hpp"
#include "wsi.hpp"
#include "train_model.hpp"
#include "libs/gzip_interface.hpp"
#include "program_option.hpp"
extern tipl::color_image bar,colormap;
int ana(void)
{
    std::string file_name = po.get("source");
    std::cout << "Read file:" << QFileInfo(file_name.c_str()).baseName().toStdString() << std::endl;
    if(!QFileInfo(file_name.c_str()).exists())
    {
        std::cout << "Cannot find file:" << file_name <<std::endl;
        return -1;
    }
    wsi w;
    if(!wsi::can_open(file_name.c_str()))
    {
        QImage I;
        if(!I.load(file_name.c_str()))
        {
            std::cout << "Cannot open image:" << file_name <<std::endl;
            return false;
        }
        QImage I2 = I.convertToFormat(QImage::Format_RGB32);
        w.rawI.resize(tipl::geometry<2>(I2.width(),I2.height()));
        std::copy((const unsigned int*)I2.bits(),(const unsigned int*)I2.bits() + w.rawI.size(),
                  w.rawI.begin());
        w.open(0);
    }
    else
    if(!w.open(file_name.c_str()))
    {
        std::cout << "Cannot open whole slide image:" << file_name <<std::endl;
        return -1;
    }

    if(po.has("color_model"))
    {
        std::cout << "Color model=" << po.get("color_model") << std::endl;
        if(!w.ml.load_from_file(po.get("color_model").c_str()))
        {
            std::cout << "Failed to load color model" << std::endl;
            return 0;
        }
    }
    if(po.has("nn"))
    {
        std::cout << "Neuroal Network=" << po.get("nn") << std::endl;
        if(!w.ml.nn.load_from_file<gz_istream>(po.get("nn").c_str()))
        {
            std::cout << "Failed to load neural network" << std::endl;
            return 0;
        }
    }

    std::vector<char> labels;
    std::vector<std::vector<float> > label_output;
    if(po.has("load_label"))
    {
        std::string param = po.get("load_label");
        std::replace(param.begin(),param.end(),',',' ');
        std::string reco_file;
        int label;
        std::istringstream in(param);
        in >> label >> reco_file;
        std::cout << "Reading text label file:" << reco_file << std::endl;
        if(w.load_text_reco_result(reco_file.c_str()))
        {
            labels.resize(w.output.size());
            std::fill(labels.begin(),labels.end(),label);
            w.output.swap(label_output);
        }
        else
            std::cout << "Fail to read target text file:" << reco_file << std::endl;
    }


    if(po.has("reco"))
    {
        bool terminated = false;
        std::cout << "Processing whole slide image. Please wait..." << std::endl;
        w.run(&terminated);
        std::cout << "Analysis completed." << std::endl;
        std::cout << "A total of " << w.output.size() << " targets were identified." << std::endl;
    }

    if(po.has("remove_repeat"))
    {
        int dis = po.get("remove_repeat",60);
        std::vector<char> remove_list(w.output.size());

        for(int i = 0;i < label_output.size();++i)
            for(int j = 0;j < w.output.size();++j)
                if(std::fabs(w.output[j][0]-label_output[i][0]) < dis &&
                   std::fabs(w.output[j][1]-label_output[i][1]) < dis)
                    remove_list[j] = 1;

        for(int i = 0;i < w.output.size();++i)
            for(int j = i+1;j < w.output.size();++j)
                if(std::fabs(w.output[j][0]-w.output[i][0]) < dis &&
                   std::fabs(w.output[j][1]-w.output[i][1]) < dis)
                    remove_list[j] = 1;
        std::vector<std::vector<float> > new_output;
        for(int i = 0;i < w.output.size();++i)
            if(!remove_list[i])
                new_output.push_back(std::move(w.output[i]));
        std::cout << "A total of " << w.output.size()-new_output.size() << " repeat targets were removed" << std::endl;
        w.output.swap(new_output);
    }
    std::multimap<float,int,std::greater<float> > sorted_result;
    if(po.has("sort_output"))
    {
        int index = po.get("sort_output",0);
        std::cout << "sorted output by parameter:" << index << std::endl;
        for(int i = 0;i < w.output.size();++i)
            sorted_result.insert(std::make_pair(w.output[i][index],i));
    }
    if(po.has("save_output"))
    {
        std::string output_filename = po.get("save_output");
        std::ofstream out(output_filename.c_str());

        int count = po.get("max_output",w.output.size());
        std::cout << "Output " << count << " features to " << output_filename << std::endl;
        auto result = sorted_result.begin();
        out << "x,y,span,area,shape,ig,cnn" << std::endl;
        for(int i = 0;i < count;++i)
        {
            int pos = i;
            if(!sorted_result.empty())
            {
                pos = result->second;
                ++result;
            }
            for(int j = 0;j < w.output[pos].size();++j)
                out << w.output[pos][j] << ",";
            out << std::endl;
        }
    }

    if(po.has("output_nn_data"))
    {
        std::string data_file = po.get("output_nn_data");
        tipl::ml::network_data<float,unsigned char> nn_data;
        std::cout << "Reading nn data file:" << data_file << std::endl;
        if(!nn_data.load_from_file<gz_istream>(data_file.c_str()))
        {
            std::cout << "Fail to read nn data file:" << data_file << std::endl;
            return false;
        }

        int output_label_count = 0;
        for(int i = 0;i < labels.size();++i)
        {
            std::vector<float> data;
            if(!w.get_picture(data,label_output[i][0],label_output[i][1],nn_data.input[0]))
                continue;
            nn_data.data.push_back(std::move(data));
            nn_data.data_label.push_back(labels[i]);
            ++output_label_count;
        }
        std::cout << "A total of " << output_label_count << " labeled targets were extracted." << std::endl;
        int count = po.get("max_output",w.output.size()+output_label_count)-output_label_count;
        if(count < 0)
            count = 0;
        int output_count = 0;
        for(int i = 0;i < count;++i)
        {
            std::vector<float> data;
            if(!w.get_picture(data,w.output[i][0],w.output[i][1],nn_data.input[0]))
            {
                count = std::min<int>(count+1,w.output.size());
                continue;
            }
            nn_data.data.push_back(std::move(data));
            nn_data.data_label.push_back(0);
            ++output_count;
        }
        std::cout << "A total of " << output_count << " reco targets were extracted." << std::endl;
        if(!nn_data.save_to_file<gz_ostream>(data_file.c_str()))
        {
            std::cout << "Fail to save to nn data file:" << data_file << std::endl;
            return false;
        }
        std::cout << "Data saved to " << data_file.c_str() << std::endl;
        return true;
    }
    return 1;
}

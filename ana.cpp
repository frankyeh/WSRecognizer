#include <QImage>
#include <QFileInfo>
#include <iostream>
#include <iterator>
#include <string>
#include "image/image.hpp"
#include "wsi.hpp"
#include "train_model.hpp"
#include "libs/gzip_interface.hpp"
#include "program_option.hpp"
extern image::color_image bar,colormap;
int ana(void)
{
    std::string file_name = po.get("source");
    std::cout << "Read file:" << QFileInfo(file_name.c_str()).baseName().toStdString() << std::endl;
    if(!QFileInfo(file_name.c_str()).exists())
    {
        std::cout << "Cannot find file:" << file_name <<std::endl;
        return -1;
    }
    if(!wsi::can_open(file_name.c_str()))
    {
        std::cout << "Cannot open file:" << file_name <<std::endl;
        return -1;
    }
    wsi w;
    if(!w.open(file_name.c_str()))
    {
        std::cout << "Cannot open whole slide image:" << file_name <<std::endl;
        return -1;
    }

    {
        std::cout << "stain 1 code" << w.color1_code << std::endl;
        std::cout << "stain 1 count=" << (int)w.color1_count << std::endl;
        std::cout << "stain 2 code" << w.color2_code << std::endl;
        std::cout << "stain 2 count=" << (int)w.color2_count << std::endl;
        std::cout << "stain ratio=" << (float)w.color1_count/(float)w.color2_count << std::endl;
    }
    if(po.has("check_stain"))
    {
        std::string text = po.get("check_stain");
        std::cout << "check stain=" << text << std::endl;
        std::replace(text.begin(),text.end(),',',' ');
        std::istringstream in(text);
        int code1,code2;
        in >> code1 >> code2;
        code1 -= w.color1_code;
        code2 -= w.color2_code;
        if(std::abs(code1) > 15 || std::abs(code2) > 15)
        {
            std::cout << "Check stain failed. The WSI has a different stain profile" << std::endl;
            return false;
        }
    }


    if(po.has("color_model") && po.has("nn"))
    {
        std::cout << "Color model=" << po.get("color_model") << std::endl;
        std::cout << "Neuroal Network=" << po.get("color_model") << std::endl;
        if(!w.ml.load_from_file(po.get("color_model").c_str()))
        {
            std::cout << "Failed to load color model" << std::endl;
            return 0;
        }
        w.ml.nn.load_from_file<gz_istream>(po.get("nn").c_str());
        bool terminated = false;
        std::cout << "Processing whole slide image. Please wait..." << std::endl;
        w.run(4000,200,std::thread::hardware_concurrency(),&terminated);
        std::string output_filename = po.get("source");
        output_filename += ".csv";
        if(po.has("output"))
            output_filename = po.get("output");
        std::multimap<float,int,std::greater<float> > sorted_result;
        for(int i = 0;i < w.result_features.size();++i)
            sorted_result.insert(std::make_pair(w.result_features[i].back(),i));
        std::ofstream out(output_filename.c_str());
        out << "x,y,span,area,shape,ig,cnn" << std::endl;
        auto result = sorted_result.begin();
        for(int i = 0;i < 100;++i,++result)
        {
            if(result == sorted_result.end())
                break;
            for(int j = 0;j < w.result_features[result->second].size();++j)
                out << w.result_features[result->second][j] << ",";
            out << std::endl;
        }
    }



    return 1;
}

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

    std::string cmd = po.get("command");
    if(cmd == "get_stain_info")
    {
        std::cout << "stain 1=(" << (int)w.color1.r << "," << (int)w.color1.g << "," << (int)w.color1.b << ")" << std::endl;
        std::cout << "stain 1 count=" << (int)w.color1_count << std::endl;
        std::cout << "stain 2=(" << (int)w.color2.r << "," << (int)w.color2.g << "," << (int)w.color2.b << ")" << std::endl;
        std::cout << "stain 2 count=" << (int)w.color2_count << std::endl;
        std::cout << "stain ratio=" << (float)w.color1_count/(float)w.color2_count << std::endl;
        return 1;
    }
    return 1;
}

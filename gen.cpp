#include <QImage>
#include <QFileInfo>
#include <iostream>
#include <iterator>
#include <string>
#include "image/image.hpp"
#include "wsi.hpp"
#include "train_model.hpp"
#include "libs/gzip_interface.hpp"
extern image::color_image bar,colormap;

int gen(int ac, char *av[])
{
    /*
    po::options_description rec_desc("recognition options");
    rec_desc.add_options()
    ("help", "help message")
    ("action", po::value<std::string>(), "gen:diffusion reconstruction trk:fiber tracking")
    ("source", po::value<std::string>(), "assign the .reg.gz file name")
    ("type", po::value<int>()->default_value(0), "assign the feature type")
    ("resolution", po::value<int>()->default_value(40), "assign the output resolution")
    ("min_value", po::value<float>()->default_value(0), "assign the min value")
    ("max_value", po::value<float>()->default_value(10), "assign the max value")
    ("min_size", po::value<float>()->default_value(0), "assign the min value")
    ("max_size", po::value<float>()->default_value(100), "assign the max value")
    ("image_type", po::value<std::string>()->default_value("tif"), "assign the output image type")
    ;

    if(!ac)
    {
        std::cout << rec_desc << std::endl;
        return 1;
    }

    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).options(rec_desc).allow_unregistered().run(), vm);
    po::notify(vm);

    wsi w;
    if(!w.load_recognition_result(vm["source"].as<std::string>().c_str()))
    {
        std::cout << "Failed to open file " << vm["source"].as<std::string>().c_str() << std::endl;
        return -1;
    }

    std::cout << "generating sdi..." << std::endl;

    int type = vm["type"].as<int>();
    int resolution = vm["resolution"].as<int>();
    float min_value = vm["min_value"].as<float>();
    float max_value = vm["max_value"].as<float>();
    float min_size = vm["min_size"].as<float>();
    float max_size = vm["max_size"].as<float>();
    std::string image_type = QString(vm["image_type"].as<std::string>().c_str()).toLower().toLocal8Bit().begin();

    std::cout << "type:" << type << std::endl;
    std::cout << "resolution:" << resolution << std::endl;
    std::cout << "min:" << min_value << std::endl;
    std::cout << "max:" << max_value << std::endl;


    image::basic_image<float,2> sdi_value;
    image::basic_image<unsigned char,2> sdi_contour;
    w.get_distribution_image(sdi_value,sdi_contour,resolution,resolution,type,min_size,max_size);

    std::cout << "image_dimension:" << sdi_value.width() << " by " << sdi_value.height() << std::endl;
    image::color_image sdi_image(sdi_value.geometry());
    if(max_value > min_value)
    {
        float r = 255.99/(max_value-min_value);
        for(unsigned int index = 0;index < sdi_value.size();++index)
        {
            int i = std::max<int>(0,std::min<int>(255,std::floor((sdi_value[index]-min_value)*r)));
            sdi_image[index] = colormap[i];
        }
    }

    if(image_type == "mat")
    {
        std::ostringstream out;
        out << vm["source"].as<std::string>()
            << ".type" << type
            << ".reso" << resolution
            << "." << image_type;
        std::string output_filename = out.str();
        std::cout << "output data to " << output_filename << std::endl;
        image::io::mat_write mat(output_filename.c_str());
        if(!mat)
        {
            std::cout << "Cannot write to file " << output_filename << std::endl;
            return 0;
        }
        mat << sdi_value;
        return 0;
    }
    else
    {
        std::ostringstream out;
        out << vm["source"].as<std::string>()
            << ".type" << type
            << ".reso" << resolution
            << ".max" << (int)max_value
            << "." << image_type;
        std::string output_filename = out.str();
        std::cout << "output figures to " << output_filename << std::endl;
        QImage I((unsigned char*)&*sdi_image.begin(),sdi_image.width(),sdi_image.height(),QImage::Format_RGB32);
        I.save(output_filename.c_str());
        return 0;
    }
    */
    return 0;
}

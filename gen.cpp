#include <QImage>
#include <QFileInfo>
#include <iostream>
#include <iterator>
#include <string>
#include "image/image.hpp"
#include "boost/program_options.hpp"
#include "wsi.hpp"
#include "train_model.hpp"
#include "libs/gzip_interface.hpp"
extern image::color_image bar,colormap;
namespace po = boost::program_options;

int gen(int ac, char *av[])
{
    po::options_description rec_desc("recognition options");
    rec_desc.add_options()
    ("help", "help message")
    ("action", po::value<std::string>(), "gen:diffusion reconstruction trk:fiber tracking")
    ("source", po::value<std::string>(), "assign the .reg.gz file name")
    ("type", po::value<int>()->default_value(0), "assign the feature type")
    ("resolution", po::value<int>()->default_value(40), "assign the output resolution")
    ("min_value", po::value<float>()->default_value(0), "assign the min value")
    ("max_value", po::value<float>()->default_value(10), "assign the max value")
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

    gz_mat_read mat;
    if(!mat.load_from_file(vm["source"].as<std::string>().c_str()))
    {
        std::cout << "Failed to open file " << vm["source"].as<std::string>().c_str() << std::endl;
        return -1;
    }
    wsi w;


    {
        unsigned int rows,cols;
        const int* dim = 0;
        const float* x = 0;
        const float* y = 0;
        const float* pixel_size = 0;
        const float* length = 0;
        if(!mat.read("dimension",rows,cols,dim))
        {
            std::cout << "Cannot find dimension matrix in " << vm["source"].as<std::string>().c_str() << std::endl;
            return -1;
        }
        std::cout << "dimension=" << dim[0] << " by " << dim[1] << std::endl;
        if(!mat.read("pixel_size",rows,cols,pixel_size))
        {
            std::cout << "Cannot find pixel_size matrix in " << vm["source"].as<std::string>().c_str() << std::endl;
            return -1;
        }
        std::cout << "pixel size=" << *pixel_size << std::endl;

        if(!mat.read("x",rows,cols,x) ||
           !mat.read("y",rows,cols,y))
        {
            std::cout << "Cannot find the location matrix in " << vm["source"].as<std::string>().c_str() << std::endl;
            return -1;
        }
        std::cout << "A total of " << cols << " targets loaded." << std::endl;

        std::cout << "Location matrix located" << std::endl;
        if(mat.read("length",rows,cols,length))
            std::cout << "Length matrix located" << std::endl;

        w.dim[0] = dim[0];
        w.dim[1] = dim[1];
        w.pixel_size = *pixel_size;
        w.result_pos.resize(cols);
        w.result_features.resize(cols);

        for(unsigned int index = 0;index < cols;++index)
        {
            w.result_pos[index][0] = x[index];
            w.result_pos[index][1] = y[index];
            if(length)
                w.result_features[index] = length[index];
        }
    }



    std::cout << "generating sdi..." << std::endl;

    int type = vm["type"].as<int>();
    int resolution = vm["resolution"].as<int>();
    float min_value = vm["min_value"].as<float>();
    float max_value = vm["max_value"].as<float>();
    std::string image_type = QString(vm["image_type"].as<std::string>().c_str()).toLower().toLocal8Bit().begin();

    std::cout << "type:" << type << std::endl;
    std::cout << "resolution:" << resolution << std::endl;
    std::cout << "min:" << min_value << std::endl;
    std::cout << "max:" << max_value << std::endl;


    image::basic_image<float,2> sdi_value;
    w.get_distribution_image(sdi_value,resolution,resolution,type);

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
    return 0;
}

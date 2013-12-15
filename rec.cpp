#include <QImage>
#include <QFileInfo>
#include <iostream>
#include <iterator>
#include <string>
#include "image/image.hpp"
#include "boost/program_options.hpp"
#include "wsi.hpp"
#include "train_model.hpp"
extern image::color_image bar,colormap;
namespace po = boost::program_options;
/**
 * @brief commandline recognition
 * @param ac
 * @param av
 * @return
 * @example: --action=rec --source=G4_SYS05-678.ndpi --background=1.bmp --foreground=2.bmp
 */
int rec(int ac, char *av[])
{
    po::options_description rec_desc("recognition options");
    rec_desc.add_options()
    ("help", "help message")
    ("action", po::value<std::string>(), "rec:diffusion reconstruction trk:fiber tracking")
    ("source", po::value<std::string>(), "assign the .src file name")
    ("background", po::value<std::string>(), "assign the mask file")
    ("foreground", po::value<std::string>(), "assign the mask file")
    ("thread_count", po::value<int>()->default_value(4), "specify number of threads")
    ("feature_type", po::value<int>()->default_value(1), "specify feature type")
    ("output_resolution", po::value<int>()->default_value(20), "specify output resolution")
    ("output_min", po::value<float>()->default_value(0), "specify output resolution")
    ("output_max", po::value<float>()->default_value(20), "specify output resolution")
    ("output", po::value<std::string>(), "specify output file name")
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
    train_model ml;

    {
        std::cout << "Loading WSI" << std::endl;
        std::string wsi_file_name = vm["source"].as<std::string>();
        if(!wsi::can_open(wsi_file_name.c_str()))
        {
            std::cout << "Invalid file format:" << wsi_file_name.c_str() << std::endl;
            return -1;
        }

        if(!w.open(wsi_file_name.c_str()))
        {
            std::cout << "Cannot open file " << wsi_file_name.c_str() << std::endl;
            return -1;
        }
        std::cout << "WSI loaded" << std::endl;
        std::cout << "width:" << w.dim[0] << " height:" << w.dim[1] << std::endl;
        std::cout << "spatial resolution:" << w.pixel_size << " micron" << std::endl;
    }

    {
        std::string filename = vm["background"].as<std::string>();
        QImage I;
        if(!I.load(filename.c_str()))
        {
            std::cout << "Invalid background image format" << std::endl;
            return -1;
        }
        QImage colorI = I.convertToFormat(QImage::Format_ARGB32);
        image::color_image data(image::geometry<2>(colorI.width(),colorI.height()));
        std::copy((const image::rgb_color*)colorI.bits(),
                  (const image::rgb_color*)colorI.bits()+data.size(),data.begin());
        ml.add_data(data,true);
        std::cout << "background image loaded." << std::endl;
    }

    {
        std::string filename = vm["foreground"].as<std::string>();
        QImage I;
        if(!I.load(filename.c_str()))
        {
            std::cout << "Invalid foreground image format" << std::endl;
            return -1;
        }
        QImage colorI = I.convertToFormat(QImage::Format_ARGB32);
        image::color_image data(image::geometry<2>(colorI.width(),colorI.height()));
        std::copy((const image::rgb_color*)colorI.bits(),
                  (const image::rgb_color*)colorI.bits()+data.size(),data.begin());
        ml.add_data(data,false);
        std::cout << "foreground image loaded." << std::endl;
    }

    std::cout << "feature_type:" << vm["feature_type"].as<int>() << std::endl;
    std::cout << "recognization using " << vm["thread_count"].as<int>() <<" thread(s)..." << std::endl;

    bool terminated = false;
    ml.predict(0);
    w.run(800,100,vm["thread_count"].as<int>(),vm["feature_type"].as<int>(),&ml,&terminated);
    std::cout << "recognition completed." << std::endl;

    image::basic_image<float,2> sdi_value;
    image::color_image sdi_image;
    {
        std::cout << "generating sdi..." << std::endl;
        std::cout << "output_resolution:" << vm["output_resolution"].as<int>() << std::endl;
        std::cout << "output_min:" << vm["output_min"].as<float>() << std::endl;
        std::cout << "output_max:" << vm["output_max"].as<float>() << std::endl;
        w.get_distribution_image(sdi_value,
                                  vm["output_resolution"].as<int>(),
                                  vm["output_resolution"].as<int>(),
                                  vm["feature_type"].as<int>());
        sdi_image.resize(sdi_value.geometry());
        float min_value = vm["output_min"].as<float>();
        float max_value = vm["output_max"].as<float>();
        if(max_value > min_value)
        {
            float r = 255.99/(max_value-min_value);
            for(unsigned int index = 0;index < sdi_value.size();++index)
            {
                int i = std::max<int>(0,std::min<int>(255,std::floor((sdi_value[index]-min_value)*r)));
                sdi_image[index] = colormap[i];
            }
        }
    }

    {
        std::string output_filename = vm["source"].as<std::string>() + ".tif";
        if(vm.count("output"))
            output_filename = vm["output"].as<std::string>();

        if(QFileInfo(output_filename.c_str()).completeSuffix() == ".mat")
        {
            image::io::mat_write mat(output_filename.c_str());
            mat.write(QFileInfo(output_filename.c_str()).baseName().toLocal8Bit().begin(),&*sdi_value.begin(),sdi_value.width(),sdi_value.height());
        }
        else
        {
            std::cout << "output figures to " << output_filename << std::endl;
            QImage I((unsigned char*)&*sdi_image.begin(),sdi_image.width(),sdi_image.height(),QImage::Format_RGB32);
            I.save(output_filename.c_str());
        }
    }

    return 0;
}


#include <QImage>
#include <QFileInfo>
#include <iostream>
#include <iterator>
#include <string>
#include "image/image.hpp"
#include "wsi.hpp"
#include "train_model.hpp"
#include "libs/gzip_interface.hpp"


int rec(int ac, char *av[])
{
    /*
    po::options_description rec_desc("recognition options");
    rec_desc.add_options()
    ("help", "help message")
    ("action", po::value<std::string>(), "rec:diffusion reconstruction trk:fiber tracking")
    ("source", po::value<std::string>(), "assign the .src file name")
    ("background", po::value<std::string>(), "assign the mask file")
    ("foreground", po::value<std::string>(), "assign the mask file")
    ("thread_count", po::value<int>()->default_value(4), "specify number of threads")
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
            std::cout << "Cannot load the background image" << std::endl;
            return -1;
        }
        QImage colorI = I.convertToFormat(QImage::Format_ARGB32);
        image::color_image data(image::geometry<2>(colorI.width(),colorI.height()));
        std::copy((const image::rgb_color*)colorI.bits(),
                  (const image::rgb_color*)colorI.bits()+data.size(),data.begin());
        w.ml.add_data(data,true);
        std::cout << "background image loaded." << std::endl;
    }

    {
        std::string filename = vm["foreground"].as<std::string>();
        QImage I;
        if(!I.load(filename.c_str()))
        {
            std::cout << "Cannot load the foreground image" << std::endl;
            return -1;
        }
        QImage colorI = I.convertToFormat(QImage::Format_ARGB32);
        image::color_image data(image::geometry<2>(colorI.width(),colorI.height()));
        std::copy((const image::rgb_color*)colorI.bits(),
                  (const image::rgb_color*)colorI.bits()+data.size(),data.begin());
        w.ml.add_data(data,false);
        std::cout << "foreground image loaded." << std::endl;
    }

    std::cout << "recognization using " << vm["thread_count"].as<int>() <<" thread(s)..." << std::endl;

    bool terminated = false;
    w.run(4000,200,vm["thread_count"].as<int>(),&terminated);
    std::cout << "recognition completed." << std::endl;
    if(w.result_pos.empty())
    {
        std::cout << "No target recognized. Process aborted. Please check the training data?" << std::endl;
        return 0;
    }
    std::cout << "A total of " << w.result_pos.size() << " targets identified" << std::endl;

    std::string output_name = vm["source"].as<std::string>();
    output_name += ".";
    output_name += QFileInfo(vm["foreground"].as<std::string>().c_str()).baseName().toLocal8Bit().begin();
    output_name += ".reg.gz";
    {
        std::cout << "Write to file" << output_name.c_str() << std::endl;
        gz_mat_write mat(output_name.c_str());
        mat.write("dimension",&*w.dim.begin(),1,2);
        mat.write("pixel_size",&w.pixel_size,1,1);
        std::vector<float> pos_x(w.result_pos.size()),pos_y(w.result_pos.size());
        for(unsigned int index = 0;index < w.result_pos.size();++index)
        {
            pos_x[index] = w.result_pos[index][0];
            pos_y[index] = w.result_pos[index][1];
        }
        mat.write("x",&*pos_x.begin(),1,pos_x.size());
        mat.write("y",&*pos_y.begin(),1,pos_x.size());
        mat.write("length",&*w.result_features.begin(),1,w.result_features.size());
    }


    */
    return 0;
}


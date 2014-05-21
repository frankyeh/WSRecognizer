#include <QApplication>
#include <QMessageBox>
#include <QLibrary>
#include <QCleanlooksStyle>
#include "boost/program_options.hpp"
#include "mainwindow.h"
#include "openslide.h"

namespace po = boost::program_options;

int rec(int ac, char *av[]);
int gen(int ac, char *av[]);


openslide_can_open_type openslide_can_open = 0;
openslide_open_type openslide_open = 0;
openslide_get_level_count_type openslide_get_level_count = 0;
openslide_get_level0_dimensions_type openslide_get_level0_dimensions = 0;
openslide_get_level_dimensions_type openslide_get_level_dimensions = 0;
openslide_get_level_downsample_type openslide_get_level_downsample = 0;
openslide_get_best_level_for_downsample_type openslide_get_best_level_for_downsample = 0;
openslide_read_region_type openslide_read_region = 0;
openslide_close_type openslide_close = 0;

openslide_get_error_type openslide_get_error = 0;
openslide_get_property_names_type openslide_get_property_names = 0;
openslide_get_property_value_type openslide_get_property_value = 0;
openslide_get_associated_image_names_type openslide_get_associated_image_names = 0;
openslide_get_associated_image_dimensions_type openslide_get_associated_image_dimensions = 0;
openslide_read_associated_image_type openslide_read_associated_image = 0;


unsigned char color_spectrum_value(unsigned char center, unsigned char value)
{
    unsigned char dif = center > value ? center-value:value-center;
    if(dif < 32)
        return 255;
    dif -= 32;
    if(dif >= 64)
        return 0;
    return 255-(dif << 2);
}
image::color_image bar,colormap;

int main(int ac, char *av[])
{
    QLibrary lib("libopenslide-0");
    if(!lib.load())
    {
        std::cout << "Cannot load open slide library." << std::endl;
        return -1;
    }
    openslide_can_open = (openslide_can_open_type) lib.resolve("openslide_can_open");
    openslide_open = (openslide_open_type) lib.resolve("openslide_open");
    openslide_get_level_count = (openslide_get_level_count_type) lib.resolve("openslide_get_level_count");
    openslide_get_level0_dimensions = (openslide_get_level0_dimensions_type) lib.resolve("openslide_get_level0_dimensions");
    openslide_get_level_dimensions = (openslide_get_level_dimensions_type) lib.resolve("openslide_get_level_dimensions");
    openslide_get_level_downsample = (openslide_get_level_downsample_type) lib.resolve("openslide_get_level_downsample");
    openslide_get_best_level_for_downsample = (openslide_get_best_level_for_downsample_type) lib.resolve("openslide_get_best_level_for_downsample");
    openslide_read_region = (openslide_read_region_type) lib.resolve("openslide_read_region");
    openslide_close = (openslide_close_type) lib.resolve("openslide_close");
    openslide_get_error = (openslide_get_error_type) lib.resolve("openslide_get_error");
    openslide_get_property_names = (openslide_get_property_names_type) lib.resolve("openslide_get_property_names");
    openslide_get_property_value = (openslide_get_property_value_type) lib.resolve("openslide_get_property_value");
    openslide_get_associated_image_names = (openslide_get_associated_image_names_type) lib.resolve("openslide_get_associated_image_names");
    openslide_get_associated_image_dimensions = (openslide_get_associated_image_dimensions_type) lib.resolve("openslide_get_associated_image_dimensions");
    openslide_read_associated_image = (openslide_read_associated_image_type) lib.resolve("openslide_read_associated_image");


    {
        colormap.resize(image::geometry<2>(256,1));
        bar.resize(image::geometry<2>(20,256));
        for(unsigned int index = 0;index < 256;++index)
        {
            image::rgb_color color;
            color.r = color_spectrum_value(64,index);
            color.g = color_spectrum_value(128,index);
            color.b = color_spectrum_value(128+64,index);
            colormap[255-index] = color;
            if(index && index != 255)
            {
                int sep = (index % 51 == 0) ? 5 : 1;
                std::fill(bar.begin()+index*20+sep,bar.begin()+(index+1)*20-sep,color);
            }
        }
    }

    if(ac > 2)
    {
        {
            std::cout << "WS Recognizer " << __DATE__ << ", Fang-Cheng Yeh" << std::endl;

        // options for general options
            po::options_description desc("options");
            desc.add_options()
            ("help", "help message")
            ("action", po::value<std::string>(), "rec:whole slide recognition")
            ("source", po::value<std::string>(), "assign the WSI file")
            ;
            po::variables_map vm;
            po::store(po::command_line_parser(ac, av).options(desc).allow_unregistered().run(),vm);
            if (vm.count("help"))
            {
                std::cout << "example: perform recognition" << std::endl;
                std::cout << "    --action=rec --source=G4_SYS05-678.ndpi --background=1.bmp --foreground=2.bmp " << std::endl;
                return 1;
            }

            if (!vm.count("action") || !vm.count("source"))
            {
                std::cout << "invalid command, use --help for more detail" << std::endl;
                return 1;
            }
            if(vm["action"].as<std::string>() == std::string("rec"))
                return rec(ac,av);
            if(vm["action"].as<std::string>() == std::string("gen"))
                return gen(ac,av);
        }
        return 1;
    }

    QApplication::setStyle(new QCleanlooksStyle);
    QApplication a(ac, av);
    a.setOrganizationName("LabSolver");
    a.setApplicationName("WS Recognizer");
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    a.setFont(font);
    MainWindow w;
    w.setFont(font);
    w.show();
    
    return a.exec();
}

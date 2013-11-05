#include <QApplication>
#include <QMessageBox>
#include <QLibrary>
#include <QCleanlooksStyle>
#include "mainwindow.h"
#include "openslide.h"
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
int main(int argc, char *argv[])
{
    QLibrary lib("libopenslide-0");
    if(!lib.load())
    {
        QMessageBox::information(0,"Error","Cannot find open slide library",0);
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


    QApplication::setStyle(new QCleanlooksStyle);
    QApplication a(argc, argv);
    a.setOrganizationName("LabSolver");
    a.setApplicationName("WSRecognizer");
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    a.setFont(font);
    MainWindow w;
    w.setFont(font);
    w.show();
    
    return a.exec();
}

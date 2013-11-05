#include "qmainscene.h"

QMainScene::QMainScene(QObject *parent) :
    QGraphicsScene(parent)
{
}


void QMainScene::update_image(void)
{
    QImage output_image;
    if(result.empty())
    {
        output_image = QImage((unsigned char*)&*main_image.begin(),main_image.width(),main_image.height(),QImage::Format_RGB32);
    }
    else
    {
        annotated_image = main_image;
        for(unsigned int index = 0;index < result.size();++index)
            if(result[index])
                annotated_image[index] = image::rgb_color(255,0,0);
        output_image = QImage((unsigned char*)&*annotated_image.begin(),annotated_image.width(),annotated_image.height(),QImage::Format_RGB32);
    }
    setSceneRect(0, 0, output_image.width(),output_image.height());
    clear();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, output_image.width(),output_image.height(),QPen(),output_image);

}

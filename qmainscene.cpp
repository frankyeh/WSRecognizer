#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "qmainscene.h"
#include "qtrainscene.h"
//#include "ui_mainwindow.h"
#include "mainwindow.h"
QMainScene::QMainScene(QObject *parent) :
    QGraphicsScene(parent),show_edge(true)
{
}

void QMainScene::clear_image(void)
{
    result.clear();
    update_image();
}

void QMainScene::update_image(void)
{
    QImage output_image;
    if(!result.empty() && show_recog)
    {
        annotated_image = main_image;
        if(show_edge)
        {
            image::grayscale_image result_edge;
            image::morphology::edge(result,result_edge);
            for(unsigned int index = 0;index < result.size();++index)
                if(result_edge[index] && !result[index])
                    annotated_image[index] = image::rgb_color(255,0,0);
        }
        else
        for(unsigned int index = 0;index < result.size();++index)
            if(result[index])
                annotated_image[index][2] = 255;
        output_image = QImage((unsigned char*)&*annotated_image.begin(),annotated_image.width(),annotated_image.height(),QImage::Format_RGB32);
        /*
        QPainter paint(&output_image);
        paint.setPen(Qt::red);
        for(unsigned int index = 0;index < result_pos.size();++index)
        {
            float r = result_features[index]/pixel_size;
            paint.drawEllipse(result_pos[index][0]-r/2.0,result_pos[index][1]-r/2.0,r,r);
        }*/
    }
    else
        output_image = QImage((unsigned char*)&*main_image.begin(),main_image.width(),main_image.height(),QImage::Format_RGB32);
    setSceneRect(0, 0, output_image.width(),output_image.height());
    clear();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, output_image.width(),output_image.height(),QPen(),output_image);

}

void QMainScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    float x = mouseEvent->scenePos().x();
    float y = mouseEvent->scenePos().y();
    std::vector<image::rgb_color> pixels;
    image::get_window(image::pixel_index<2>(x,y,main_image.geometry()),main_image,pixels);
    for(unsigned int index = 0;index < pixels.size();++index)
        train_scene->ml.add_data(pixels[index],(mouseEvent->button() == Qt::LeftButton ? 0:1));
    train_scene->ml.update_classifier_map();
    train_scene->update();
    mouseEvent->accept();

}


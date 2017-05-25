#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include "qmainscene.h"
#include "qtrainscene.h"
//#include "ui_mainwindow.h"
#include "mainwindow.h"
QMainScene::QMainScene(QObject *parent) :
    QGraphicsScene(parent),show_edge(true),w(0),level(0)
{
}

void QMainScene::clear_image(void)
{
    if(!w)
        return;
    result.clear();
    update_image();
}
void QMainScene::move_to(unsigned int x_,unsigned int y_)
{
    if(!w)
    {
        if(!main_image.empty()) // if single image is loaded
            update_image();
        return;
    }
    x = x_;
    y = y_;
    main_image.resize(image::geometry<2>(std::max(100,views()[0]->width()-20),std::max(100,views()[0]->height()-20)));
    w->read(main_image,x,y,level);
    result.clear();
    update_image();
}

void QMainScene::update_image(void)
{
    QImage output_image;
    if(main_image.empty())
    {
        clear();
        return;
    }
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

        QPainter paint(&output_image);
        paint.setPen(Qt::blue);
        for(unsigned int index = 0;index < result_features.size();++index)
        if(result_features[index][target::span] > 0) // size not 0
        {
            float r = result_features[index][target::span]/pixel_size;
            paint.drawEllipse(result_features[index][target::pos_x]-r/2.0,result_features[index][target::pos_y]-r/2.0,r,r);
        }
    }
    else
        output_image = QImage((unsigned char*)&*main_image.begin(),main_image.width(),main_image.height(),QImage::Format_RGB32);

    if(!w)
        output_image = output_image.scaled(output_image.width()*(level+1),output_image.height()*(level+1));
    setSceneRect(0, 0, output_image.width(),output_image.height());
    clear();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, output_image.width(),output_image.height(),QPen(),output_image);

}
void QMainScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    moved = false;
    if(!w)
        return;
    lx = mouseEvent->scenePos().x();
    ly = mouseEvent->scenePos().y();
}

void QMainScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    moved = true;
    if(!w)
        return;
    float dx = mouseEvent->scenePos().x()-lx;
    float dy = mouseEvent->scenePos().y()-ly;
    if(level)
    {
        float ratio = w->dim[0]/w->dim_at_level[level][0];
        dx *= ratio;
        dy *= ratio;
    }
    move_to(x - dx,y - dy);
    lx = mouseEvent->scenePos().x();
    ly = mouseEvent->scenePos().y();
}

void QMainScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(main_image.empty() || moved)
        return;
    float x = mouseEvent->scenePos().x();
    float y = mouseEvent->scenePos().y();
    if(!w) // if single image is loaded
    {
        x /= level+1;
        y /= level+1;
    }
    std::vector<image::rgb_color> pixels;
    image::get_window(image::pixel_index<2>(x,y,main_image.geometry()),main_image,pixels);
    for(unsigned int index = 0;index < pixels.size();++index)
        train_scene->ml.add_data(pixels[index],(mouseEvent->button() == Qt::LeftButton ? 0:1));
    train_scene->ml.update_classifier_map();
    train_scene->update();
    mouseEvent->accept();

}


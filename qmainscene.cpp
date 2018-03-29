#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include "qmainscene.h"
#include "qtrainscene.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

void QMainScene::clear_image(void)
{
    if(!w)
        return;
    result.clear();
    update_image();
}
void QMainScene::move_to(int x_,int y_)
{
    if(!w)
    {
        if(!main_image.empty()) // if single image is loaded
            update_image();
        return;
    }
    int iw = std::min<int>(w->get_width(level),std::max(100,views()[0]->width()-20));
    int ih = std::min<int>(w->get_height(level),std::max(100,views()[0]->height()-20));
    x = std::max<int>(0,std::min<int>(w->dim[0]-iw*w->get_r(level),x_));
    y = std::max<int>(0,std::min<int>(w->dim[1]-ih*w->get_r(level),y_));
    if(level < 0)
    {
        for(int i = 0;i > level;--i)
        {
            iw /= 2;
            ih /= 2;
        }
        main_image.resize(image::geometry<2>(iw,ih));
        w->read(main_image,x,y,0);
        for(int i = 0;i > level;--i)
            image::upsampling(main_image);
    }
    else
    {
        main_image.resize(image::geometry<2>(iw,ih));
        w->read(main_image,x,y,level);
    }
    result.clear();
    update_image();
    main_window->map_scene.update();
}
void QMainScene::zoom_in(void)
{
    if(!w || level < -1)
        return;
    --level;
    move_to(x+main_image.width()*(w->get_r(level+1)-w->get_r(level))/2,
            y+main_image.height()*(w->get_r(level+1)-w->get_r(level))/2);
}

void QMainScene::zoom_out(void)
{
    if(!w || level == w->level-1)
        return;
    ++level;
    move_to(x+main_image.width()*(w->get_r(level-1)-w->get_r(level))/2,
            y+main_image.height()*(w->get_r(level-1)-w->get_r(level))/2);
}

void QMainScene::update_image(void)
{
    QImage output_image;
    if(main_image.empty())
    {
        clear();
        return;
    }
    if(show_recog)
    {
        annotated_image = main_image;
        if(!result.empty())
        {
            image::grayscale_image result_edge;
            image::morphology::edge(result,result_edge);
            for(unsigned int index = 0;index < result.size();++index)
                if(result_edge[index] && !result[index])
                    annotated_image[index] = image::rgb_color(255,0,0);
        }
        output_image = QImage((unsigned char*)&*annotated_image.begin(),annotated_image.width(),annotated_image.height(),QImage::Format_RGB32);

        QPainter paint(&output_image);
        paint.setPen(QPen(Qt::blue,2));

        float r = w->get_r(level);
        int x_to = x+width()*r;
        int y_to = y+height()*r;
        int pen_size = 25.0f/r;
        for(unsigned int index = 0;index < main_window->result_features.size();++index)
        if(main_window->result_features[index][0] > x &&
           main_window->result_features[index][0] < x_to &&
           main_window->result_features[index][1] > y &&
           main_window->result_features[index][1] < y_to) // size not 0
        {
            int px = (main_window->result_features[index][0]-x)/r;
            int py = (main_window->result_features[index][1]-y)/r;
            if(pen_size > 5)
                paint.drawEllipse(px-pen_size,py-pen_size,pen_size+pen_size,pen_size+pen_size);
            else
            {
                paint.drawLine(px-3,py,px+3,py);
                paint.drawLine(px,py-3,px,py+3);
            }
        }
    }
    else
        output_image = QImage((unsigned char*)&*main_image.begin(),main_image.width(),main_image.height(),QImage::Format_RGB32);
    if(!w)
        output_image = output_image.scaled(output_image.width()*(level+1),output_image.height()*(level+1));
    else
    {
        float size = w->get_pixel_size(level);
        float scale[8] = {5,10,50,100,500,1000,5000,10000};
        char text[8][13] = {"5 microns","10 microns","50 microns",
                            "100 microns","500 microns","1 mm","5 mm","1 cm"};
        int scale_id = 0;
        float length = scale[scale_id]/size;
        while(1)
        {
            length = scale[scale_id]/size;
            if(scale_id == 7 || length > 50)
                break;
            ++scale_id;
        }
        QPainter p(&output_image);
        p.setPen(QPen(Qt::black,2));
        p.drawLine(20,20,20+length,20);
        p.drawLine(20,15,20,25);
        p.drawLine(20+length,15,20+length,25);
        p.drawText(30+length,20,text[scale_id]);
    }
    setSceneRect(0, 0, output_image.width(),output_image.height());
    clear();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, output_image.width(),output_image.height(),QPen(),output_image);

}
void QMainScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(mouseEvent->button() == Qt::LeftButton)
    {
        float r = w->get_r(level);
        std::vector<float> new_location(7);
        new_location[0] = mouseEvent->scenePos().x()*r+x;
        new_location[1] = mouseEvent->scenePos().y()*r+y;
        std::vector<std::vector<float> > result;
        result.push_back(new_location);
        main_window->addRecoResult(result);
    }
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
        float ratio = w->get_r(level);
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

    if(main_window->ui->train_classifier->isChecked())
    {
        train_scene->ml.add_data(main_image[image::pixel_index<2>(x,y,main_image.geometry()).index()],
                                            (mouseEvent->button() == Qt::LeftButton ? 0:1));
        train_scene->update();
    }
    mouseEvent->accept();

}


#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QPainter>
#include "qmapgraphicsscene.h"
#include "qmainscene.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

void QMapGraphicsScene::update(void)
{
    map_image = w->map_image;
    //if(show_mask)
    {
        tipl::grayscale_image edge(w->map_mask);
        tipl::morphology::edge(edge);
        for(unsigned int index = 0;index < edge.size();++index)
            if(edge[index])
                map_image[index] = tipl::rgb(255,0,0);
    }
    //if(main_window->ui->show_reco_on_map->isChecked())
    {
        double rx = double(map_image.width()-1)/w->dim[0];
        double ry = double(map_image.height()-1)/w->dim[1];
        int shift[9] = {-map_image.width()*2,-map_image.width(),-2,-1,0,1,2,map_image.width(),map_image.width()*2};
        for(int i = 0;i < main_window->output.size();++i)
        {
            bool red = (i == main_window->ui->recog_result->currentRow());
            int x = main_window->output[i][0]*rx;
            int y = main_window->output[i][1]*ry;
            int pos = y*map_image.width()+x;
            for(int j = 0;j < 9;++j)
            {
                int p = pos + shift[j];
                if(p >=0 && p < map_image.size())
                    map_image[p] = red ? tipl::rgb(255,0,0) : tipl::rgb(0,0,255);
            }
        }
    }


    QImage I((unsigned char*)&*map_image.begin(),map_image.width(),map_image.height(),QImage::Format_RGB32);


    qimage = I.scaledToWidth(map_image.width()*10.0f/(float)resolution);
    {
        QPainter paint(&qimage);
        double x = qimage.width()*main_scene.x/w->dim[0];
        double y = qimage.height()*main_scene.y/w->dim[1];
        double rx = qimage.width()*main_scene.main_image.width()*w->get_r(main_scene.level)/w->dim[0];
        double ry = qimage.height()*main_scene.main_image.height()*w->get_r(main_scene.level)/w->dim[1];
        paint.setPen(Qt::red);
        paint.drawLine(x,y,x+rx,y);
        paint.drawLine(x,y+ry,x+rx,y+ry);
        paint.drawLine(x,y,x,y+ry);
        paint.drawLine(x+rx,y,x+rx,y+ry);
    }
    setSceneRect(0, 0, qimage.width(),qimage.height());
    clear();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, qimage.width(),qimage.height(),QPen(),qimage);
}

void QMapGraphicsScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(mouseEvent->button() == Qt::RightButton)
        right_button = true;
    else
        right_button = false;

    sel_point.clear();
}

void QMapGraphicsScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    float Y = mouseEvent->scenePos().y();
    float X = mouseEvent->scenePos().x();
    QImage bitmap(qimage);
    QPainter paint(&bitmap);
    sel_point.push_back(tipl::vector<2,short>(X, Y));
    for (unsigned int index = 1; index < sel_point.size(); ++index)
        paint.drawLine(sel_point[index-1][0], sel_point[index-1][1],sel_point[index][0], sel_point[index][1]);
    clear();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, bitmap.width(),bitmap.height(),QPen(),bitmap);
}

void QMapGraphicsScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(!w || w->map_image.empty())
        return;

    if (sel_point.size() > 2)
    {
        QImage bitmap(qimage.width(),qimage.height(),QImage::Format_Mono);
        {
            QPainter paint(&bitmap);
            paint.setBrush(Qt::black);
            paint.drawRect(0,0,bitmap.width(),bitmap.height());
            paint.setBrush(Qt::white);
            std::vector<QPoint> qpoints(sel_point.size());
            for(unsigned int index = 0;index < sel_point.size();++index)
                qpoints[index] = QPoint(sel_point[index][0],sel_point[index][1]);
            paint.drawPolygon(&*qpoints.begin(),qpoints.size() - 1);
        }
        QImage draw_map = bitmap.scaled(w->map_mask.width(),w->map_mask.height());
        for (tipl::pixel_index<2>index(w->map_mask.geometry());index < w->map_mask.size();++index)
            if(QColor(draw_map.pixel(index.x(),index.y())).red() > 64)
            {
                if(right_button)
                    w->map_mask[index.index()] = 0;
                else
                    w->map_mask[index.index()] = 1;
            }
        update();
    }
    else
    {
        int x = w->dim[0]*mouseEvent->scenePos().x()/(sceneRect().width()-1);
        int y = w->dim[1]*mouseEvent->scenePos().y()/(sceneRect().height()-1);
        main_scene.move_to(x,y);
    }
    sel_point.clear();







}

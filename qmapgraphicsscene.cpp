#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QPainter>
#include "qmapgraphicsscene.h"
#include "qmainscene.h"

void QMapGraphicsScene::update(void)
{
    map_image = w->map_image;
    if(show_mask)
    {
        image::grayscale_image edge(w->map_mask);
        image::morphology::edge(edge);
        for(unsigned int index = 0;index < edge.size();++index)
            if(edge[index])
                map_image[index] = image::rgb_color(255,0,0);
    }
    QImage I((unsigned char*)&*map_image.begin(),map_image.width(),map_image.height(),QImage::Format_RGB32);
    qimage = I.scaled(w->dim[0]*w->pixel_size/(float)resolution,w->dim[1]*w->pixel_size/(float)resolution);
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
    sel_point.push_back(image::vector<2,short>(X, Y));
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
    main_scene.move_to(w->dim[0]*mouseEvent->scenePos().x()/(sceneRect().width()-1),
                       w->dim[1]*mouseEvent->scenePos().y()/(sceneRect().height()-1));

    if(sel_point.empty())
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
        for (image::pixel_index<2>index(w->map_mask.geometry());index < w->map_mask.size();++index)
            if(QColor(draw_map.pixel(index.x(),index.y())).red() > 64)
            {
                if(right_button)
                    w->map_mask[index.index()] = 0;
                else
                    w->map_mask[index.index()] = 1;
            }
    }
    sel_point.clear();
    update();
}

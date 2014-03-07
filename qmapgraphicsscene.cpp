#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
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
    QImage qimage = I.scaled(w->dim[0]*w->pixel_size/(float)resolution,w->dim[1]*w->pixel_size/(float)resolution);
    setSceneRect(0, 0, qimage.width(),qimage.height());
    clear();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, qimage.width(),qimage.height(),QPen(),qimage);
}

void QMapGraphicsScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(!w)
        return;
    float x = mouseEvent->scenePos().x()/(sceneRect().width()-1);
    float y = mouseEvent->scenePos().y()/(sceneRect().height()-1);
    main_scene.main_image.resize(image::geometry<2>(
            std::max(100,main_scene.views()[0]->width()-20),
            std::max(100,main_scene.views()[0]->height()-20)));
    w->read(main_scene.main_image,
            std::max<int>(0,x*w->dim[0] - main_scene.sceneRect().width()/2.0),
            std::max<int>(0,y*w->dim[1] - main_scene.sceneRect().height()/2.0));
    main_scene.result.clear();
    main_scene.update_image();
}

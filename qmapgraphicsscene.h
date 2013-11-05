#ifndef QMAPGRAPHICSSCENE_H
#define QMAPGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "wsi.hpp"
class QMainScene;
class QMapGraphicsScene : public QGraphicsScene
{
    QMainScene& main_scene;
    image::color_image map_image;
public:
    wsi* w;
    bool show_mask;
    QMapGraphicsScene(QMainScene& main_scene_):main_scene(main_scene_),show_mask(true){}
protected:
    //void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
public:
    void update(void);
};

#endif // QMAPGRAPHICSSCENE_H

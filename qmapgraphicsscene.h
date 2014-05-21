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
    QImage qimage;
    bool show_mask;
    unsigned int resolution;
    QMapGraphicsScene(QMainScene& main_scene_):main_scene(main_scene_),show_mask(true),resolution(40){}
protected:
    bool right_button;
    std::vector<image::vector<2,short> >sel_point;
    //void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
public:
    void update(void);
};

#endif // QMAPGRAPHICSSCENE_H

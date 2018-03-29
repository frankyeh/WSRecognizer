#ifndef QMAPGRAPHICSSCENE_H
#define QMAPGRAPHICSSCENE_H

#include <QtWidgets/QGraphicsScene>
#include "wsi.hpp"
class QMainScene;
class MainWindow;
class QMapGraphicsScene : public QGraphicsScene
{
    QMainScene& main_scene;
    image::color_image map_image;
public:
    MainWindow* main_window;
    wsi* w;
    QImage qimage;
    unsigned int resolution;
    QMapGraphicsScene(QMainScene& main_scene_,MainWindow* main_window_):
        main_scene(main_scene_),main_window(main_window_),resolution(40){}
protected:
    bool right_button;
    std::vector<image::vector<2,short> >sel_point;
    void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
public:
    void update(void);
};

#endif // QMAPGRAPHICSSCENE_H

#ifndef QMAINSCENE_H
#define QMAINSCENE_H

#include <QtWidgets/QGraphicsScene>
#include "image/image.hpp"

class MainWindow;
class QTrainScene;
class wsi;
class QMainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    wsi* w;
    int level;
    int x,y;
    float pixel_size;
    image::color_image main_image, annotated_image;
    bool show_recog;
    QTrainScene* train_scene;
    MainWindow* main_window;
    QMainScene(MainWindow *main_window_):
        show_edge(true),w(0),level(0),main_window(main_window_)
    {
    }
public:
    bool show_edge;
    image::grayscale_image result;

    void reload(void)
    {
        move_to(x,y);
    }
    void move_to(int x_,int y_);
    void zoom_in(void);
    void zoom_out(void);
protected:
    bool moved;
    unsigned int lx,ly;
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );

signals:
    
public slots:
    void update_image(void);
    void clear_image(void);
private slots:
};

#endif // QMAINSCENE_H

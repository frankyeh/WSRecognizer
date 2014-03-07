#ifndef QMAINSCENE_H
#define QMAINSCENE_H

#include <QGraphicsScene>
#include "image/image.hpp"

class MainWindow;
class QTrainScene;

class QMainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    float pixel_size;
    image::color_image main_image, annotated_image;
    bool show_recog;
    QTrainScene* train_scene;
    explicit QMainScene(QObject *parent = 0);
public:
    bool show_edge;
    image::grayscale_image result;
    std::vector<image::vector<2> > result_pos;
    std::vector<float> result_features;
protected:
    //void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
signals:
    
public slots:
    void update_image(void);
    void clear_image(void);
private slots:
};

#endif // QMAINSCENE_H

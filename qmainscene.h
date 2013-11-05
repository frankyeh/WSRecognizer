#ifndef QMAINSCENE_H
#define QMAINSCENE_H

#include <QGraphicsScene>
#include "image/image.hpp"

class QMainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    image::color_image main_image, annotated_image;
    image::grayscale_image result;
    explicit QMainScene(QObject *parent = 0);
protected:
    //void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
signals:
    
public slots:
    void update_image(void);
};

#endif // QMAINSCENE_H

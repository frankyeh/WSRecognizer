#ifndef QTRAINSCENE_H
#define QTRAINSCENE_H

#include <QtWidgets/QGraphicsScene>
#include "train_model.hpp"
class MainWindow;
class QTrainScene : public QGraphicsScene
{
    Q_OBJECT
public:
    train_model ml;
public:
    MainWindow* main_window;
    QTrainScene(MainWindow*);
protected:
    //void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    //void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
signals:
    
public slots:
    void update(void);
};

#endif // QTRAINSCENE_H

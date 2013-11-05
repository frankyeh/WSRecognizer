#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "qtrainscene.h"


QTrainScene::QTrainScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void QTrainScene::update(void)
{
    clear();
    if(ml.is_empty())
        return;
    QImage qimage((unsigned char*)&*ml.classifier_map.begin(),ml.classifier_map.width(),ml.classifier_map.height(),QImage::Format_RGB32);
    {
        QPainter paint(&qimage);
        image::vector<2,double> x_dir,y_dir,z_dir;
        x_dir[0] = ml.r[0];
        x_dir[1] = ml.r[3];
        y_dir[0] = ml.r[1];
        y_dir[1] = ml.r[4];
        z_dir[0] = ml.r[2];
        z_dir[1] = ml.r[5];
        int ox,oy,x,y;
        size_t image_size = std::min(qimage.width(),qimage.height());
        train_model::get_position(image::vector<3,double>(0,0,0),x_dir,y_dir,z_dir,image_size,ox,oy);

        train_model::get_position(image::vector<3,double>(1.0,0,0),x_dir,y_dir,z_dir,image_size,x,y);
        paint.setPen(Qt::red);
        paint.drawLine(ox,oy,x,y);

        train_model::get_position(image::vector<3,double>(0.0,1.0,0),x_dir,y_dir,z_dir,image_size,x,y);
        paint.setPen(Qt::green);
        paint.drawLine(ox,oy,x,y);

        train_model::get_position(image::vector<3,double>(0.0,0,1.0),x_dir,y_dir,z_dir,image_size,x,y);
        paint.setPen(Qt::blue);
        paint.drawLine(ox,oy,x,y);
    }
    setSceneRect(0, 0, qimage.width(),qimage.height());
    setItemIndexMethod(QGraphicsScene::NoIndex);
    addRect(0, 0, qimage.width(),qimage.height(),QPen(),qimage);
}

void QTrainScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    double dx = mouseEvent->scenePos().x()-mouseEvent->lastScenePos().x();
    double dy = mouseEvent->scenePos().y()-mouseEvent->lastScenePos().y();
    double rr[9],tmp[9];
    image::rotation_y_matrix(-dx* 0.01,rr);
    image::matrix::product(rr,ml.r,tmp,image::dyndim(3,3),image::dyndim(3,3));
    image::rotation_x_matrix(dy* 0.01,rr);
    image::matrix::product(rr,tmp,ml.r,image::dyndim(3,3),image::dyndim(3,3));
    ml.update_classifier_map();
    update();
}

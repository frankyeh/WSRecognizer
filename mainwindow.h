#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QSettings>
#include "qmapgraphicsscene.h"
#include "qtrainscene.h"
#include "qmainscene.h"
#include "wsi.hpp"
#include "boost/thread.hpp"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    std::auto_ptr<wsi> w;
    QMapGraphicsScene map_scene;
    QTrainScene train_scene;
    QMainScene main_scene;
    QGraphicsScene info_scene,result_scene;
    image::color_image main_image;
private:
    std::auto_ptr<boost::thread> thread;
    std::auto_ptr<QTimer> timer;
    bool terminated;
private:
    image::basic_image<float,2> sdi_value;
    image::color_image sdi_image;
    QGraphicsScene color_bar;
private slots:
    void show_run_progress(void);
private:
    QString work_path,file_name;
    QSettings settings;
    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];
    void updateRecentList(QStringList files);
    void openFile(QString);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event);
private slots:
    void on_action_Open_triggered();


    void on_recognize_stains_clicked();

    void on_run_clicked();

    void on_show_map_mask_toggled(bool checked);

    void on_save_reco_result_clicked();

    void openRecentFile(void);

    void update_sdi(void);
    void update_color_bar(void);

    void on_open_reco_result_clicked();


    void on_update_image_clicked();

    void on_new_model_clicked();

    void on_open_model_clicked();

    void on_save_model_clicked();

    void on_show_recog_toggled(bool checked);

    void set_training_param();

    void on_actionBatch_analysis_triggered();

    void on_actionBatch_generate_images_triggered();

public:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

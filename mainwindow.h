#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/qmainwindow.h>
#include <QTableWidgetItem>
#include <QTimer>
#include <QSettings>
#include "qmapgraphicsscene.h"
#include "qtrainscene.h"
#include "qmainscene.h"
#include "wsi.hpp"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    std::auto_ptr<wsi> w;
    QMapGraphicsScene map_scene;
    QTrainScene train_scene;
    QMainScene main_scene;
    QGraphicsScene info_scene,result_scene,data_scene,nn_scene;
    tipl::color_image main_image;
public:
    std::vector<std::vector<float> > output;
public:
    bool end_training = false;
    float test_error,training_error;
    tipl::thread nn_thread;
    tipl::ml::network_data<unsigned char> nn_data;
    QImage data_image,data_image2,nn_image;
    std::auto_ptr<QTimer> nn_timer;
private:
    tipl::thread thread;
    std::auto_ptr<QTimer> timer;
    bool terminated;
    unsigned int added_results;
    QStringList reco_result_list;
private:
    tipl::image<float,2> sdi_value;
    tipl::image<unsigned char,2> sdi_contour;
    tipl::color_image sdi_image;
    QGraphicsScene color_bar;
private slots:
    void show_run_progress(void);
private:
    QString work_path,file_name,reg_name;
    QSettings settings;
    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];
    void updateRecentList(QStringList files);
    void openFile(QString);
    void on_add_nn_data_clicked(int label);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event);
    void wheelEvent ( QWheelEvent * event );
    void keyPressEvent ( QKeyEvent * event );
    void addRecoResult(const std::vector<std::vector<float> >& result);

public slots:
    void on_nn_timer();
    void show_nn(const std::vector<float>& data,unsigned char label);

private slots:
    void on_action_Open_triggered();


    void on_recognize_stains_clicked();

    void on_run_clicked();

    void openRecentFile(void);

    void update_sdi(void);
    void update_color_bar(void);
    void update_result();





    void on_new_model_clicked();

    void on_show_recog_toggled(bool checked);

    void set_training_param();

    void on_actionBatch_analysis_triggered();

    void on_actionBatch_generate_images_triggered();

    void on_map_size_valueChanged(int value);




    void on_actionOpen_image_triggered();

    void on_pushButton_clicked();

    void on_tma_feature_currentIndexChanged(int index);



    void on_open_reco_clicked();

    void on_save_reco_clicked();

    void on_save_density_image_clicked();


    void on_del_row_clicked();

    void on_del_all_clicked();

    void on_nn_filter_results_clicked();

    void on_apply_intensity_normalizatoin_clicked();

    void on_actionLoad_Stain_Classifier_triggered();

    void on_actionSave_Stain_Classifier_triggered();

    void on_actionSmoothing_triggered();

    void on_actionEnlarge_triggered();

    void on_actionDefragment_triggered();

    void on_actionShrink_triggered();

    void on_actionClear_triggered();

    void on_clear_all_clicked();

    void on_actionThreshold_triggered();

    void on_actionLoad_Neural_Network_triggered();


    void on_load_nn_data_clicked();

    void on_save_nn_data_clicked();

    void on_train_nn_clicked();

    void on_reset_nn_clicked();




    void on_data_pos_valueChanged(int value);

    void on_del_nn_data_clicked();

    void on_actionSave_Neural_Network_triggered();

    void on_clear_nn_clicked();

    void on_resolve_nn_data_clicked();

    void on_add_negative_nn_data_clicked();

    void on_add_positive_nn_data_clicked();

    void on_actionBatch_quality_check_triggered();

    void on_sort_result_by_currentIndexChanged(int index);


    void on_actionFlip_X_triggered();

    void on_actionFlip_Y_triggered();

    void on_actionSwap_XY_triggered();

    void on_recog_result_itemSelectionChanged();

public:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

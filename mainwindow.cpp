#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QProgressDialog>
#include <QAction>
#include "wsi.hpp"
#include "rec_dialog.hpp"
#include "gen_dialog.hpp"

extern std::auto_ptr<QProgressDialog> progressDialog;
extern image::color_image bar,colormap;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    map_scene(main_scene)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&map_scene);
    ui->info_view->setScene(&info_scene);
    ui->main_view->setScene(&main_scene);
    ui->train_view->setScene(&train_scene);
    ui->result_view->setScene(&result_scene);
    ui->info_widget->setColumnWidth(0,200);
    ui->info_widget->setColumnWidth(1,200);
    ui->splitter->setSizes(QList<int> () << height()*3/4 << height()/4);

    main_scene.train_scene = &train_scene;
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
        ui->menuRecent_Files->addAction(recentFileActs[i]);
    }



    QObject::connect(ui->type,SIGNAL(currentIndexChanged(int)),this,SLOT(update_result()));
    QObject::connect(ui->contour,SIGNAL(clicked()),this,SLOT(update_result()));
    QObject::connect(ui->color_min,SIGNAL(valueChanged(double)),this,SLOT(update_color_bar()));
    QObject::connect(ui->color_max,SIGNAL(valueChanged(double)),this,SLOT(update_color_bar()));
    QStringList recent_file_list = settings.value("recent_files").toStringList();
    updateRecentList(recent_file_list);
    if(!recent_file_list.isEmpty())
        work_path = QFileInfo(recent_file_list[0]).absolutePath();

    progressDialog.reset(new QProgressDialog);
}

MainWindow::~MainWindow()
{
    if(thread.get())
        on_run_clicked();
    delete ui;
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    main_scene.setSceneRect(0,0,ui->main_view->width()-5,ui->main_view->height()-5);
}

void MainWindow::updateRecentList(QStringList files)
{
    // remove those files no readable
    for(size_t index = 0;index < files.count();)
        if(!QFileInfo(files[index]).isReadable())
            files.removeAt(index);
        else
            ++index;

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
    for (int i = 0; i < numRecentFiles; ++i)
    {
        recentFileActs[i]->setText(QString("&%1 %2").arg(i + 1).arg(files[i]));
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}

void MainWindow::openRecentFile(void)
{
    QAction *action = qobject_cast<QAction *>(sender());
    openFile(action->data().toString());
}

void MainWindow::openFile(QString filename)
{
    std::auto_ptr<wsi> new_w(new wsi);
    if(!wsi::can_open(filename.toLocal8Bit().begin()) ||
       !new_w->open(filename.toLocal8Bit().begin()))
    {
        QMessageBox::information(this,"error","Cannot open file",0);
        return;
    }
    w.reset(new_w.release());
    map_scene.w = w.get();
    map_scene.update();
    if(!w->associated_image.empty())
    {
        QImage qimage((unsigned char*)&*w->associated_image[0].begin(),w->associated_image[0].width(),w->associated_image[0].height(),QImage::Format_RGB32);
        info_scene.setSceneRect(0, 0, qimage.width(),qimage.height());
        info_scene.clear();
        info_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
        info_scene.addRect(0, 0, qimage.width(),qimage.height(),QPen(),qimage);
    }

    ui->info_widget->clear();
    ui->info_widget->setHorizontalHeaderLabels(QStringList() << "property" << "name");
    ui->info_widget->setRowCount(w->property_name.size());
    for(unsigned int index = 0;index < ui->info_widget->rowCount();++index)
    {
        ui->info_widget->setItem(index, 0, new QTableWidgetItem(w->property_name[index].c_str()));
        ui->info_widget->setItem(index, 1, new QTableWidgetItem(w->property_value[index].c_str()));
    }

    main_scene.main_image.clear();
    main_scene.pixel_size = w->pixel_size;
    main_scene.update_image();


    work_path = QFileInfo(filename).absolutePath();
    file_name = filename;
    setWindowTitle(file_name);
    // update recent file list

    QStringList files = settings.value("recent_files").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    while (files.size() > MaxRecentFiles)
         files.removeLast();
    settings.setValue("recent_files", files);
    updateRecentList(files);
    setWindowTitle(filename);

    QString resultfile = filename+".txt";
    if(QFileInfo(resultfile).exists())
    {
        w->load_text_reco_result(resultfile.toLocal8Bit().begin());
        update_sdi();
        update_color_bar();
    }
}

void MainWindow::on_action_Open_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open WSI",work_path,"WSI files (*.svs *.tif *.vms *.vmu *.scn *.mrxs *.ndpi);;All files (*)");
    if (filename.isEmpty())
        return;
    openFile(filename);
}

void MainWindow::set_training_param()
{
    if(!train_scene.ml.is_trained() || !w.get())
        return;
    train_scene.ml.smoothing = ui->smoothing->value();
}

void MainWindow::on_recognize_stains_clicked()
{
    if(!train_scene.ml.is_trained() || !w.get() || main_scene.main_image.empty())
        return;
    set_training_param();
    train_scene.ml.recognize(main_scene.main_image,main_scene.result);
    main_scene.result_pos.clear();
    main_scene.result_features.clear();
    train_scene.ml.cca(main_scene.result,w->pixel_size,0,main_scene.result_pos,main_scene.result_features);
    ui->show_recog->setChecked(true);
    main_scene.update_image();
}

void MainWindow::on_run_clicked()
{
    if(!w.get())
        return;
    set_training_param();
    if(thread.get())
    {
        disconnect(timer.get(), SIGNAL(timeout()), this, SLOT(show_run_progress()));
        terminated = true;
        thread->join();
        ui->progressBar->setValue(0);
        ui->run->setText("Run");
        thread.reset(0);
        return;
    }
    terminated = false;
    w->ml = train_scene.ml;
    thread.reset(new boost::thread(&wsi::run,w.get(),4000,200,ui->thread_count->value(),&terminated));
    timer.reset(new QTimer(this));
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(show_run_progress()));
    timer->start(1500);
    ui->progressBar->setValue(0);
    ui->run->setText("Stop");
}




void MainWindow::show_run_progress(void)
{
    ui->progressBar->setValue(w->progress);
    if(thread.get() && w->finished)
        on_run_clicked();
    update_sdi();
    update_color_bar();
}
void MainWindow::update_sdi(void)
{
    w->get_distribution_image(sdi_value,ui->resolution->value(),ui->resolution->value(),ui->type->currentIndex(),ui->min_size->value(),ui->max_size->value());
}

void MainWindow::update_color_bar(void)
{
    result_scene.clear();
    result_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    int right = 0;
    int buttom = 0;
    // main image
    if(!sdi_value.empty())
    {
        sdi_image.resize(sdi_value.geometry());
        float min_value = ui->color_min->value();
        float max_value = ui->color_max->value();
        if(max_value > min_value)
        {
            float r = 255.99/(max_value-min_value);
            for(unsigned int index = 0;index < sdi_value.size();++index)
            {
                int i = std::max<int>(0,std::min<int>(255,std::floor((sdi_value[index]-min_value)*r)));
                sdi_image[index] = colormap[i];
            }
        }
        if(ui->contour->isChecked())
            w->add_contour(sdi_image);
        result_scene.addPixmap(QPixmap::fromImage(
                QImage((unsigned char*)&*sdi_image.begin(),
                       sdi_image.width(),sdi_image.height(),QImage::Format_RGB32)))->moveBy(0,10);


        right = sdi_image.width();
        buttom = sdi_image.height()+10;


        // show 5 mm
        QPen pen;
        pen.setWidth(2);
        result_scene.addLine(0,buttom+5,5000/ui->resolution->value(),buttom+5,pen);
        result_scene.addText("5 mm")->moveBy(2500/ui->resolution->value()-12,buttom+10);
        buttom += 50;
    }
    // color bar
    {
        int shift = 0;
        if(sdi_value.height() > 255)
            shift = sdi_value.height()-255;
        result_scene.addText(QString::number(ui->color_max->value()))->moveBy(right+25,shift);
        for(int i = 1;i < 5;++i)
            result_scene.addText(QString::number(
                                     (ui->color_max->value()-ui->color_min->value())*(5.0-(float)i)/5.00)+
                                      ui->color_min->value())->moveBy(right+25,256*i/5-3+shift);
        result_scene.addText(QString::number(ui->color_min->value()))->moveBy(right+25,256+shift);
        QGraphicsTextItem *unit_text = result_scene.addText("counts per 100 square micron");
        unit_text->rotate(90);
        unit_text->moveBy(right+65,64+shift);
        buttom = std::max<int>(buttom,275);
        result_scene.addPixmap(QPixmap::fromImage(
                QImage((unsigned char*)&*bar.begin(),bar.width(),bar.height(),QImage::Format_RGB32)))->moveBy(right+5,10+shift);
        right += 65;
    }

    result_scene.setSceneRect(0, 0, right,buttom);
    ui->result_view->show();
}

void MainWindow::on_show_map_mask_toggled(bool checked)
{
    if(w.get())
    {
        map_scene.show_mask = checked;
        map_scene.update();
    }
}

void MainWindow::on_save_reco_result_clicked()
{
    if(!w.get() || w->result_pos.empty())
        return;
    QString filename = QFileDialog::getSaveFileName(
                           this,"Save results",file_name + ".tif","TIF file (*.tif);;MAT file (*.mat);;text files (*.txt);;All files (*)");
    if (filename.isEmpty())
        return;
    if(QFileInfo(filename).suffix() == "txt")
    {
        std::ofstream out(filename.toLocal8Bit().begin());
        for(unsigned int index = 0;index < w->result_pos.size();++index)
        {
            out << w->result_pos[index][0] << " " << w->result_pos[index][1] << " "
                << w->result_features[index] << std::endl;
        }
        return;
    }

    if(QFileInfo(filename).suffix() == "mat")
    {
        image::io::mat_write mat(filename.toLocal8Bit().begin());
        mat << sdi_value;
        return;
    }
    if(QFileInfo(filename).suffix() == "tif")
    {
        QImage I((unsigned char*)&*sdi_image.begin(),sdi_image.width(),sdi_image.height(),QImage::Format_RGB32);
        I.save(filename);
        return;
    }

}

void MainWindow::on_open_reco_result_clicked()
{
    if(!w.get())
        return;
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Save image",file_name + ".txt","text files (*.txt);;All files (*)");
    if (filename.isEmpty())
        return;
    w->load_text_reco_result(filename.toLocal8Bit().begin());
    update_sdi();
    update_color_bar();
}


void MainWindow::update_result()
{
    if(!w.get())
        return;
    update_sdi();
    update_color_bar();
}


void MainWindow::on_new_model_clicked()
{
    train_scene.ml.clear();
    train_scene.update();
    main_scene.clear_image();
}

void MainWindow::on_open_model_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open image",file_name + ".mdl.gz","text files (*.mdl.gz);;All files (*)");
    if (filename.isEmpty())
        return;
    if(!train_scene.ml.load_from_file(filename.toLocal8Bit().begin()))
    {
        QMessageBox::information(this,"Error","Invalid file format",0);
        return;
    }
    ui->smoothing->setValue(train_scene.ml.smoothing);
}

void MainWindow::on_save_model_clicked()
{
    if(!w.get())
        return;
    QString filename = QFileDialog::getSaveFileName(
                           this,
                           "Save image",file_name + ".mdl.gz","text files (*.mdl.gz);;All files (*)");
    if (filename.isEmpty())
        return;
    train_scene.ml.save_to_file(filename.toLocal8Bit().begin());
}

void MainWindow::on_show_recog_toggled(bool checked)
{
    main_scene.show_recog = checked;
    main_scene.update_image();
}


void MainWindow::on_actionBatch_analysis_triggered()
{
    (new rec_dialog(this,work_path))->show();
}

void MainWindow::on_actionBatch_generate_images_triggered()
{
    (new gen_dialog(this,work_path))->show();
}

void MainWindow::on_map_size_valueChanged(int value)
{
    map_scene.resolution = value;
    map_scene.update();
}

void MainWindow::on_dilation_clicked()
{
    image::morphology::dilation(w->map_mask);
    map_scene.update();
}

void MainWindow::on_erosion_clicked()
{
    image::morphology::erosion(w->map_mask);
    map_scene.update();
}

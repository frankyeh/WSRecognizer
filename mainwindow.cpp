#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QAction>
#include "wsi.hpp"


unsigned char color_spectrum_value(unsigned char center, unsigned char value)
{
    unsigned char dif = center > value ? center-value:value-center;
    if(dif < 32)
        return 255;
    dif -= 32;
    if(dif >= 64)
        return 0;
    return 255-(dif << 2);
}

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

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
        ui->menuRecent_Files->addAction(recentFileActs[i]);
    }

    {
        colormap.resize(image::geometry<2>(256,1));
        bar.resize(image::geometry<2>(20,256));
        for(unsigned int index = 0;index < 256;++index)
        {
            image::rgb_color color;
            color.r = color_spectrum_value(64,index);
            color.g = color_spectrum_value(128,index);
            color.b = color_spectrum_value(128+64,index);
            colormap[255-index] = color;
            if(index && index != 255)
                std::fill(bar.begin()+index*20+1,bar.begin()+(index+1)*20-1,color);
        }
    }

    QObject::connect(ui->color_min,SIGNAL(valueChanged(double)),this,SLOT(update_color_bar()));
    QObject::connect(ui->color_max,SIGNAL(valueChanged(double)),this,SLOT(update_color_bar()));
    updateRecentList(settings.value("recent_files").toStringList());
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

void MainWindow::on_open_training_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open training image",work_path,"image files (*.bmp *.png *.tif);;All files (*)");
    if (filename.isEmpty())
        return;
    QImage I;
    if(!I.load(filename))
    {
        QMessageBox::information(this,"Error","Invalid image format",0);
        return;
    }
    QImage colorI = I.convertToFormat(QImage::Format_ARGB32);
    image::color_image data(image::geometry<2>(colorI.width(),colorI.height()));
    std::copy((const image::rgb_color*)colorI.bits(),
              (const image::rgb_color*)colorI.bits()+data.size(),data.begin());
    train_scene.ml.add_data(data,ui->stain_type->currentIndex());
    train_scene.update();
}

void MainWindow::on_recognize_stains_clicked()
{
    if(train_scene.ml.is_empty() || !w.get() || main_scene.main_image.empty())
        return;
    train_scene.ml.recognize(main_scene.main_image,main_scene.result);
    image::morphology::edge(main_scene.result);
    main_scene.update_image();
}

void MainWindow::on_run_clicked()
{
    if(!w.get())
        return;
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
    unsigned int featue_type = 1; // size
    terminated = false;
    thread.reset(new boost::thread(&wsi::run,w.get(),800,100,
                ui->thread_count->value(),featue_type,&train_scene.ml,&terminated));
    timer.reset(new QTimer(this));
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(show_run_progress()));
    timer->start(1000);
    ui->progressBar->setValue(0);
    ui->run->setText("Stop");
}




void MainWindow::show_run_progress(void)
{
    ui->progressBar->setValue(w->progress);
    if(terminated && thread.get())
    {
        on_run_clicked();
        update_sdi();
        update_color_bar();
    }
}
void MainWindow::update_sdi(void)
{
    w->get_distribution_image(sdi_value,ui->zoom->value(),false);
}

void MainWindow::update_color_bar(void)
{
    result_scene.clear();
    int right = 0;
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
        QImage output_image;
        output_image = QImage((unsigned char*)&*sdi_image.begin(),sdi_image.width(),sdi_image.height(),QImage::Format_RGB32);
        result_scene.setSceneRect(0, 0, output_image.width(),output_image.height());
        result_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
        result_scene.addRect(0, 0, output_image.width(),output_image.height(),QPen(),output_image);
        right = output_image.width();
    }
    QGraphicsTextItem *max_text = result_scene.addText(QString::number(ui->color_max->value()));
    QGraphicsTextItem *min_text = result_scene.addText(QString::number(ui->color_min->value()));
    QGraphicsPixmapItem *map = result_scene.addPixmap(QPixmap::fromImage(
            QImage((unsigned char*)&*bar.begin(),bar.width(),bar.height(),QImage::Format_RGB32)));
    max_text->moveBy(right+25,-10);
    min_text->moveBy(right+25,246);
    map->moveBy(right+5,0);
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
                           this,"Save image",file_name + ".txt","text files (*.txt);;All files (*)");
    if (filename.isEmpty())
        return;
    std::ofstream out(filename.toLocal8Bit().begin());
    for(unsigned int index = 0;index < w->result_pos.size();++index)
    {
        out << w->result_pos[index][0] << " " << w->result_pos[index][1] << " "
            << w->result_features[index] << std::endl;
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

    std::ifstream in(filename.toLocal8Bit().begin());
    std::string line;
    while(std::getline(in,line))
    {
        double x,y,v;
        std::istringstream values(line);
        values >> x >> y >> v;
        w->result_pos.push_back(image::vector<2>(x,y));
        w->result_features.push_back(v);
    }
    update_sdi();
    ui->color_max->setValue(*std::max_element(sdi_value.begin(),sdi_value.end()));
    update_color_bar();
}

void MainWindow::on_zoom_valueChanged(int arg1)
{
    update_sdi();
    update_color_bar();
}

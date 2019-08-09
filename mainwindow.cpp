#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QProgressDialog>
#include <QAction>
#include <QInputDialog>
#include <QClipboard>
#include "wsi.hpp"
#include "rec_dialog.hpp"
#include "gen_dialog.hpp"
#include "libs/prog_interface_static_link.h"
#include "libs/gzip_interface.hpp"

extern tipl::color_image bar,colormap;
void show_info_dialog(const std::string& title,const std::string& result)
{
    QMessageBox msgBox;
    msgBox.setText(title.c_str());
    msgBox.setDetailedText(result.c_str());
    msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Save);
    msgBox.setDefaultButton(QMessageBox::Ok);
    QPushButton *copyButton = msgBox.addButton("Copy To Clipboard", QMessageBox::ActionRole);
    if(msgBox.exec() == QMessageBox::Save)
    {
        QString filename;
        filename = QFileDialog::getSaveFileName(0,"Save as","report.txt","Text files (*.txt);;All files|(*)");
        if(filename.isEmpty())
            return;
        std::ofstream out(filename.toLocal8Bit().begin());
        out << result.c_str();
    }
    if (msgBox.clickedButton() == copyButton)
        QApplication::clipboard()->setText(result.c_str());
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    map_scene(main_scene,this),
    main_scene(this),
    train_scene(this)
{
    reco_result_list << "X" << "Y" << "Span" << "Area" << "Shape" << "Intensity" << "NN";
    ui->setupUi(this);
    ui->tma_feature->clear();
    for(int i = 2;i < feature_count;++i)
        ui->tma_feature->addItem(feature_list[i]);

    ui->graphicsView->setScene(&map_scene);
    ui->info_view->setScene(&info_scene);
    ui->main_view->setScene(&main_scene);
    ui->train_view->setScene(&train_scene);
    ui->result_view->setScene(&result_scene);
    ui->data_view->setScene(&data_scene);
    ui->nn_view->setScene(&nn_scene);
    ui->info_widget->setColumnWidth(0,200);
    ui->info_widget->setColumnWidth(1,200);
    ui->result_widget->setTabEnabled(2,false);
    main_scene.train_scene = &train_scene;
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
        ui->menuRecent_Files->addAction(recentFileActs[i]);
    }

    ui->sort_result_by->addItems(reco_result_list);


    QObject::connect(ui->type,SIGNAL(currentIndexChanged(int)),this,SLOT(update_result()));
    QObject::connect(ui->contour,SIGNAL(clicked()),this,SLOT(update_result()));
    QObject::connect(ui->flip,SIGNAL(clicked()),this,SLOT(update_color_bar()));
    QObject::connect(ui->color_min,SIGNAL(valueChanged(double)),this,SLOT(update_color_bar()));
    QObject::connect(ui->color_max,SIGNAL(valueChanged(double)),this,SLOT(update_color_bar()));
    QObject::connect(ui->orientation,SIGNAL(valueChanged(int)),this,SLOT(update_color_bar()));
    QObject::connect(ui->resolution,SIGNAL(valueChanged(int)),this,SLOT(update_sdi()));
    QObject::connect(ui->resolution,SIGNAL(valueChanged(int)),this,SLOT(update_color_bar()));

    QObject::connect(ui->intensity_homo,SIGNAL(toggled(bool)),this,SLOT(on_apply_intensity_normalizatoin_clicked()));
    QObject::connect(ui->normalized_intensity,SIGNAL(valueChanged(int)),this,SLOT(on_apply_intensity_normalizatoin_clicked()));
    QObject::connect(ui->stain1_scale,SIGNAL(valueChanged(double)),this,SLOT(on_apply_intensity_normalizatoin_clicked()));
    QObject::connect(ui->stain2_scale,SIGNAL(valueChanged(double)),this,SLOT(on_apply_intensity_normalizatoin_clicked()));

    QObject::connect(ui->load_model,SIGNAL(clicked()),this,SLOT(on_actionLoad_Stain_Classifier_triggered()));
    QObject::connect(ui->save_model,SIGNAL(clicked()),this,SLOT(on_actionSave_Stain_Classifier_triggered()));
    QObject::connect(ui->load_nn,SIGNAL(clicked()),this,SLOT(on_actionLoad_Neural_Network_triggered()));
    QObject::connect(ui->save_nn,SIGNAL(clicked()),this,SLOT(on_actionSave_Neural_Network_triggered()));
    QObject::connect(ui->sort_incremental,SIGNAL(clicked()),this,SLOT(on_sort_result_by_currentIndexChanged()));

    QStringList recent_file_list = settings.value("recent_files").toStringList();
    updateRecentList(recent_file_list);
    if(!recent_file_list.isEmpty())
        work_path = QFileInfo(recent_file_list[0]).absolutePath();
}

MainWindow::~MainWindow()
{
    if(thread.has_started())
        on_run_clicked();
    delete ui;
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    main_scene.setSceneRect(0,0,ui->main_view->width()-5,ui->main_view->height()-5);
    main_scene.reload();
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
        main_scene.result.clear();

        QImage I;
        if(!I.load(filename))
        {
            QMessageBox::information(this,"Error","Cannot open image file",0);
            return;
        }
        QImage I2 = I.convertToFormat(QImage::Format_RGB32);
        new_w->rawI.resize(tipl::geometry<2>(I2.width(),I2.height()));
        std::copy((const unsigned int*)I2.bits(),(const unsigned int*)I2.bits() + new_w->rawI.size(),
                  new_w->rawI.begin());
        new_w->open(0);
    }
    ui->recog_result->setRowCount(0);
    output.clear();

    ui->NavigationWidget->show();
    ui->result_widget->setTabEnabled(0,true);
    ui->result_widget->setTabEnabled(1,true);

    w.reset(new_w.release());


    map_scene.w = w.get();
    map_scene.update();

    {
        QImage qimage;
        if(!w->associated_image.empty())
        {
            qimage = QImage((unsigned char*)&*w->associated_image[0].begin(),
                w->associated_image[0].width(),w->associated_image[0].height(),QImage::Format_RGB32).scaledToWidth(150);
        }
        else
            qimage = QImage(150,150,QImage::Format_RGB32);
        info_scene.setSceneRect(0, 0, qimage.width()+60,std::max<int>(qimage.height(),160));
        info_scene.clear();
        info_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
        info_scene.addRect(0, 0, qimage.width(),qimage.height(),QPen(),qimage);
        QImage S1(35,35,QImage::Format_RGB32),S2(35,35,QImage::Format_RGB32);
        S1.fill(QColor(w->color1.color));
        S2.fill(QColor(w->color2.color));
        info_scene.addText(QString("Stain: %1").arg(w->color1_code))->moveBy(qimage.width()+7,0);
        info_scene.addRect(qimage.width()+10,20,35,35,QPen(),S1);
        info_scene.addText(QString("Stain: %1").arg(w->color2_code))->moveBy(qimage.width()+7,55);
        info_scene.addRect(qimage.width()+10,75,35,35,QPen(),S2);
        ui->slice_tab_widget->setMaximumHeight(std::max<int>(qimage.height(),140)+100);
    }

    ui->result_widget->setTabEnabled(2,w->is_tma);
    ui->info_widget->clear();
    ui->info_widget->setHorizontalHeaderLabels(QStringList() << "property" << "name");
    ui->info_widget->setRowCount(w->property_name.size());
    for(unsigned int index = 0;index < ui->info_widget->rowCount();++index)
    {
        ui->info_widget->setItem(index, 0, new QTableWidgetItem(w->property_name[index].c_str()));
        ui->info_widget->setItem(index, 1, new QTableWidgetItem(w->property_value[index].c_str()));
    }
    work_path = QFileInfo(filename).absolutePath();
    QDir::setCurrent(work_path);
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

    /*
    QString resultfile = filename+".txt";
    if(QFileInfo(resultfile).exists())
    {
        w->load_text_reco_result(resultfile.toLocal8Bit().begin());
        update_sdi();
        update_color_bar();
    }
    */
    main_scene.level = 0;
    main_scene.w = w.get();
    main_scene.x = w->dim[0]/2;
    main_scene.y = w->dim[1]/2;
    main_scene.main_image.clear();
    main_scene.pixel_size = w->pixel_size;
\
    on_apply_intensity_normalizatoin_clicked();

}

void MainWindow::on_action_Open_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open WSI",work_path,"WSI files (*.svs *.tif *.bmp *.jpg *.vms *.vmu *.scn *.mrxs *.ndpi);;All files (*)");
    if (filename.isEmpty())
        return;


    openFile(filename);
}

void MainWindow::set_training_param()
{
    if(!train_scene.ml.is_trained())
        return;
    train_scene.ml.param[0] = ui->smoothing->value();
    train_scene.ml.param[1] = ui->check_span->isChecked() ? ui->min_span->value() : 0;
    train_scene.ml.param[2] = ui->check_span->isChecked() ? ui->max_span->value() : 0;
    train_scene.ml.param[3] = ui->check_area->isChecked() ? ui->min_area->value() : 0;
    train_scene.ml.param[4] = ui->check_area->isChecked() ? ui->max_area->value() : 0;
    train_scene.ml.param[5] = ui->check_shape->isChecked() ? ui->min_shape->value() : 0;
    train_scene.ml.param[6] = ui->check_shape->isChecked() ? ui->max_shape->value() : 0;
    train_scene.ml.param[7] = ui->check_edge->isChecked() ? ui->min_edge->value() : 0;
    train_scene.ml.param[8] = ui->check_edge->isChecked() ? ui->max_edge->value() : 0;

}

void MainWindow::on_recognize_stains_clicked()
{
    if(!train_scene.ml.is_trained() || main_scene.main_image.empty())
        return;
    if(main_scene.level <= 0)
    {
        set_training_param();
        std::vector<std::vector<float> > new_features;
        train_scene.ml.recognize(main_scene.main_image,main_scene.result);

        train_scene.ml.cca(main_scene.main_image,main_scene.result,w.get() ? w->pixel_size : 1,
                           0,main_scene.x,main_scene.y,new_features,0,true,w->color_m_inv);
        ui->test_result->setText(QString("%1 targets recognized").arg(new_features.size()));
        ui->show_recog->setChecked(true);
        main_scene.update_image();
        addRecoResult(new_features);
    }
    else
    {
        QMessageBox::information(this,"Error","Please zoom in at higher power view to recognize targets",0);
    }
}

void MainWindow::on_sort_result_by_currentIndexChanged(int index)
{
    if(output.empty())
        return;
    int sort_index = ui->sort_result_by->currentIndex();
    if(sort_index == -1)
        return;
    if(ui->sort_incremental->isChecked())
        std::sort(output.begin(), output.end(),
        [&](const std::vector<float>& a,const std::vector<float>& b)
            {
                return a[sort_index] < b[sort_index];
            });
    else
        std::sort(output.begin(), output.end(),
        [&](const std::vector<float>& a,const std::vector<float>& b)
            {
                return a[sort_index] > b[sort_index];
            });
    ui->recog_result->setRowCount(0);
    std::vector<std::vector<float> > sorted_result;
    sorted_result.swap(output);
    addRecoResult(sorted_result);
}
void MainWindow::addRecoResult(const std::vector<std::vector<float> >& result)
{
    if(!ui->target_location->isChecked())
        return;
    ui->recog_result->setHorizontalHeaderLabels(reco_result_list);

    for(unsigned int index = 0;index < result.size();++index)
    {
        output.push_back(result[index]);
        if(ui->recog_result->rowCount() > 65535)
            continue;
        ui->recog_result->setRowCount(ui->recog_result->rowCount()+1);
        int row = ui->recog_result->rowCount()-1;
        for(int i = 0;i < 7;++i)
            ui->recog_result->setItem(row, i,
                new QTableWidgetItem(QString::number((i < result[index].size()) ? result[index][i] : 0)));

    }
    map_scene.update();
    main_scene.update_image();
}

void MainWindow::on_run_clicked()
{
    if(!w.get())
        return;
    set_training_param();
    if(thread.has_started())
    {
        disconnect(timer.get(), SIGNAL(timeout()), this, SLOT(show_run_progress()));
        terminated = true;
        thread.wait();
        thread.clear();
        ui->progressBar->setValue(0);
        ui->run->setText("Run");
        return;
    }
    added_results = 0;
    terminated = false;
    w->ml.nn = train_scene.ml.nn;
    w->ml = train_scene.ml;
    thread.clear();
    thread.run([this]()
    {
        w->run(&terminated);
    });
    timer.reset(new QTimer(this));
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(show_run_progress()));
    timer->start(5000);
    ui->progressBar->setValue(0);
    ui->run->setText("Stop");
}

void MainWindow::show_run_progress(void)
{
    if(added_results < w->output.size())
    {
        int count = w->output.size();
        std::vector<std::vector<float> > new_result;
        for(int i = added_results;i < count;++i)
            new_result.push_back(w->output[i]);
        added_results = count;
        addRecoResult(new_result);
    }
    ui->progressBar->setValue(w->progress);
    if(thread.has_started() && w->finished)
        on_run_clicked();
    update_sdi();
    update_color_bar();
}
void MainWindow::update_sdi(void)
{
    w->get_distribution_image(sdi_value,sdi_contour,ui->resolution->value(),ui->resolution->value(),
                              ui->type->currentIndex());


    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(4);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("mean"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(w->mean_value)));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("max"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem(QString::number(w->max_value)));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("q1"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem(QString::number(w->q1_value)));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("q3"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(QString::number(w->q3_value)));

    if(w->is_tma)
    {
        ui->tma_table->clear();
        ui->tma_table->setColumnCount(w->tma_array.width());
        ui->tma_table->setRowCount(w->tma_array.height());
        for(int i = 0;i < w->tma_array.width();++i)
            ui->tma_table->setColumnWidth(i,50);
        for(int i = 0,index = 0;i < w->tma_array.height();++i)
            for(int j = 0;j < w->tma_array.width();++j,++index)
            {
                int m = ui->label_on_right->isChecked() ? w->tma_array.size() - index -1 : index;
                ui->tma_table->setItem(i,j,new QTableWidgetItem(w->tma_array[m] ?
                                                                QString::number(w->tma_results[w->tma_array[m]-1][ui->tma_feature->currentIndex()]):
                                                                QString(" ")));
            }
    }
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
        {
            for(unsigned int index = 0;index < sdi_contour.size();++index)
                if(sdi_contour[index])
                    sdi_image[index] = 0;

        }

        {
            QImage I = QImage((unsigned char*)&*sdi_image.begin(),
               sdi_image.width(),sdi_image.height(),QImage::Format_RGB32);
            QPixmap main_image;
            main_image = QPixmap::fromImage(ui->flip->isChecked()? I.mirrored():I);
            if(ui->orientation->value() != 0)
            {
                QPixmap output(main_image.size());
                output.fill(Qt::darkBlue);
                QPainter p(&output);
                p.setRenderHint(QPainter::Antialiasing);
                p.setRenderHint(QPainter::SmoothPixmapTransform);
                p.setRenderHint(QPainter::HighQualityAntialiasing);
                p.translate(output.size().width() / 2, output.size().height() / 2);
                p.rotate(ui->orientation->value()*10);
                p.translate(-output.size().width() / 2, -output.size().height() / 2);
                p.drawPixmap(0, 0, main_image);
                p.end();
                main_image = output;
            }
            result_scene.addPixmap(main_image)->moveBy(0,10);
            right = main_image.width();
            buttom = main_image.height()+10;
        }




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
        unit_text->setRotation(270);
        unit_text->moveBy(right+65,64+shift);
        buttom = std::max<int>(buttom,275);
        result_scene.addPixmap(QPixmap::fromImage(
                QImage((unsigned char*)&*bar.begin(),bar.width(),bar.height(),QImage::Format_RGB32)))->moveBy(right+5,10+shift);
        right += 65;
    }

    result_scene.setSceneRect(0, 0, right,buttom);
    ui->result_view->show();
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
    if(!w.get())
        return;
    map_scene.resolution = value;
    map_scene.update();
}


void MainWindow::on_actionOpen_image_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open Image",work_path,"Image files (*.tif *.bmp *.jpg);;All files (*)");
    if (filename.isEmpty())
        return;

}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                           this,
                           "Save as",QFileInfo(file_name).baseName() + reg_name + ".tma.txt","Text files (*.txt);;All files (*)");
    if (filename.isEmpty())
        return;
    w->save_tma_result(filename.toStdString().c_str(),ui->label_on_right->isChecked());
}

void MainWindow::on_tma_feature_currentIndexChanged(int index)
{
    if(!w.get())
        return;
    update_sdi();
}

void MainWindow::on_open_reco_clicked()
{
    if(!w.get())
        return;
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open results",work_path + "//"+ QFileInfo(file_name).baseName() + reg_name + ".txt","location files (*.txt *.csv *.xml);;All files (*)");
    if (filename.isEmpty())
        return;
    if(QFileInfo(filename).suffix().toLower() == "xml")
    {
        std::shared_ptr<QFile> xmlFile(new QFile(filename));
        if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this,"Load XML File Problem",
            "Couldn't open the file",QMessageBox::Ok);
            return;
        }
        std::shared_ptr<QXmlStreamReader> xmlReader(new QXmlStreamReader(xmlFile.get()));

        std::vector<std::vector<float> > output;
        while(!xmlReader->atEnd() && !xmlReader->hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader->readNext();
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
                //If token is StartElement - read it
                if(token == QXmlStreamReader::StartElement)
                {
                    if(xmlReader->name() == "Vertex") {
                        std::vector<float> f(7);
                        f[0] = xmlReader->attributes().value("X").toFloat();
                        f[1] = xmlReader->attributes().value("Y").toFloat();
                        output.push_back(f);
                    }
                }
        }
        xmlReader->clear();
        xmlFile->close();
        w->output.swap(output);
    }
    else
        w->load_text_reco_result(filename.toLocal8Bit().begin());
    if(w->output.size() > 0)
    {
        ui->recog_result->setRowCount(0);
        output.clear();
        addRecoResult(w->output);
        update_sdi();
        update_color_bar();
    }
}

void MainWindow::on_save_reco_clicked()
{
    if(!w.get() || ui->recog_result->rowCount() == 0)
        return;
    QString filename = QFileDialog::getSaveFileName(
                           this,"Save results",work_path + "//"+ QFileInfo(file_name).baseName() + reg_name + ".txt","text files (*.txt);;jpg files (*.jpg);;data (*.bin);;All files (*)");
    if (filename.isEmpty())
        return;

    if(QFileInfo(filename).suffix() == "jpg")
    {
        for(int row = 0;row < output.size();++row)
        {
            tipl::color_image I;
            if(!w->get_picture(I,output[row][0],output[row][1],200))
                continue;
            QImage qimage((unsigned char*)&*I.begin(),I.width(),I.height(),QImage::Format_RGB32);
            qimage.save(QFileInfo(filename).absolutePath() + "/" +
                        QFileInfo(filename).baseName() + "_" +
                        QString("%1").arg(row, 5, 10, QChar('0')) + ".jpg");
        }
        return;
    }
    std::ofstream out(filename.toLocal8Bit().begin());
    for(int row = 0;row < output.size();++row)
    {
        for(int col = 0;col < output[row].size();++col)
            out << output[row][col] << " ";
        out << std::endl;
    }
}

void MainWindow::on_save_density_image_clicked()
{
    if(!w.get() || w->output.empty())
        return;
    QString filename = QFileDialog::getSaveFileName(
                           this,"Save results",work_path + "//"+ QFileInfo(file_name).baseName() + reg_name + ".jpg","jpg image (*.jpg);;All files (*)");
    if (filename.isEmpty())
        return;
    QImage I((unsigned char*)&*sdi_image.begin(),sdi_image.width(),sdi_image.height(),QImage::Format_ARGB32);
    I.save(filename);
}

void MainWindow::on_del_row_clicked()
{
    if(output.empty())
        return;
    int row = ui->recog_result->currentRow();
    if(row < 0)
        return;
    ui->recog_result->removeRow(row);
    output.erase(output.begin()+row);

    on_recog_result_itemSelectionChanged();
    ui->recog_result->setFocus();
}

void MainWindow::on_del_all_clicked()
{
    ui->recog_result->setRowCount(0);
    output.clear();
    map_scene.update();
    main_scene.update_image();
}


void MainWindow::wheelEvent ( QWheelEvent * event )
{
    if(!w.get())
        return;
    if(event->delta() > 0)
        main_scene.zoom_in();
    else
        main_scene.zoom_out();
}
void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    if(!nn_data.empty() && event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
    {
        int label = event->key()-Qt::Key_0;
        nn_data.data_label[ui->data_pos->value()] = label;
        on_data_pos_valueChanged(0);
        event->accept();
        return;
    }

    if(w.get())
    switch(event->key())
    {
        case Qt::Key_A:
            main_scene.x -= main_scene.width()*w->get_r(main_scene.level)/4;
            main_scene.reload();
            event->accept();
            break;
        case Qt::Key_D:
            main_scene.x += main_scene.width()*w->get_r(main_scene.level)/4;
            main_scene.reload();
            event->accept();
            break;
        case Qt::Key_W:
            main_scene.y -= main_scene.height()*w->get_r(main_scene.level)/4;
            main_scene.reload();
            event->accept();
            break;
        case Qt::Key_S:
            main_scene.y += main_scene.height()*w->get_r(main_scene.level)/4;
            main_scene.reload();
            event->accept();
            break;


        /*
        case Qt::Key_Left:
            glWidget->move_by(-1,0);
            break;
        case Qt::Key_Right:
            glWidget->move_by(1,0);
            break;
        case Qt::Key_Up:
            glWidget->move_by(0,-1);
            break;
        case Qt::Key_Down:
            glWidget->move_by(0,1);
            break;*/
    }
    if(event->isAccepted())
        return;
    QWidget::keyPressEvent(event);
}

void MainWindow::on_nn_filter_results_clicked()
{
    if(!w.get() || train_scene.ml.nn.empty())
        return;
    for(int row = 0;row < output.size();++row)
    {
        int x = output[row][0];
        int y = output[row][1];
        std::vector<float> data;
        if(!w->get_picture(data,x,y,train_scene.ml.nn.get_input_dim()[0]))
            continue;
        if(!train_scene.ml.nn.predict(data))
        {
            ui->recog_result->removeRow(row);
            output.erase(output.begin()+row);
            --row;
        }
    }
}

void MainWindow::on_apply_intensity_normalizatoin_clicked()
{
    if(!w.get())
        return;
    w->intensity_normalization = ui->intensity_homo->isChecked();
    w->intensity_norm_value = ui->normalized_intensity->value();
    w->set_stain_scale(ui->stain1_scale->value(),ui->stain2_scale->value());
    main_scene.reload();
}

void MainWindow::on_actionLoad_Stain_Classifier_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open image",work_path + "//"+ QFileInfo(file_name).baseName() + ".mdl.gz","text files (*.mdl.gz);;All files (*)");
    if (filename.isEmpty())
        return;
    if(!train_scene.ml.load_from_file(filename.toLocal8Bit().begin()))
    {
        QMessageBox::information(this,"Error","Invalid file format",0);
        return;
    }
    reg_name = QFileInfo(filename).baseName();
    ui->smoothing->setValue(train_scene.ml.param[0]);
    ui->check_span->setChecked(train_scene.ml.param[1] != 0.0f || train_scene.ml.param[2] != 0.0f);
    ui->check_area->setChecked(train_scene.ml.param[3] != 0.0f || train_scene.ml.param[4] != 0.0f);
    ui->check_shape->setChecked(train_scene.ml.param[5] != 0.0f || train_scene.ml.param[6] != 0.0f);
    ui->check_edge->setChecked(train_scene.ml.param[7] != 0.0f || train_scene.ml.param[8] != 0.0f);


    ui->min_span->setValue(train_scene.ml.param[1]);
    ui->max_span->setValue(train_scene.ml.param[2]);
    ui->min_area->setValue(train_scene.ml.param[3]);
    ui->max_area->setValue(train_scene.ml.param[4]);
    ui->min_shape->setValue(train_scene.ml.param[5]);
    ui->max_shape->setValue(train_scene.ml.param[6]);
    ui->min_edge->setValue(train_scene.ml.param[7]);
    ui->max_edge->setValue(train_scene.ml.param[8]);

}

void MainWindow::on_actionSave_Stain_Classifier_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
                           this,
                           "Save image",work_path + "//"+ QFileInfo(file_name).baseName() + reg_name + ".mdl.gz","text files (*.mdl.gz);;All files (*)");
    if (filename.isEmpty())
        return;
    set_training_param();
    train_scene.ml.save_to_file(filename.toLocal8Bit().begin());
}

void MainWindow::on_actionSmoothing_triggered()
{
    tipl::morphology::smoothing(w->map_mask);
    map_scene.update();
}

void MainWindow::on_actionEnlarge_triggered()
{
    tipl::morphology::dilation(w->map_mask);
    map_scene.update();
}

void MainWindow::on_actionDefragment_triggered()
{
    tipl::morphology::defragment(w->map_mask);
    map_scene.update();
}

void MainWindow::on_actionShrink_triggered()
{
    tipl::morphology::erosion(w->map_mask);
    map_scene.update();
}

void MainWindow::on_actionClear_triggered()
{
    std::fill(w->map_mask.begin(),w->map_mask.end(),0);
    map_scene.update();
}


void MainWindow::on_clear_all_clicked()
{
    std::fill(w->map_mask.begin(),w->map_mask.end(),0);
    map_scene.update();
}

void MainWindow::on_actionThreshold_triggered()
{
    bool ok;
    tipl::grayscale_image gray_image(w->map_image);
    int threshold = QInputDialog::getInt(this,"DSI Studio","Please assign the threshold",
                                         (int)tipl::segmentation::otsu_threshold(gray_image),
                                         0,255,1,&ok);
    if (!ok)
        return;
    w->map_mask = gray_image;
    tipl::binary(w->map_mask,std::bind2nd (std::less<unsigned char>(), threshold));
        map_scene.update();
}

void MainWindow::on_actionLoad_Neural_Network_triggered()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(
                this,
                "Open network",work_path,
                "Network files (*.net *.net.gz);;All files (*)");
    if(filename.isEmpty())
        return;
    if(nn_thread.has_started())
    {
        nn_thread.clear();
        nn_timer->stop();
    }
    train_scene.ml.nn.load_from_file<gz_istream>(filename.toStdString().c_str());
    ui->nn->setText(train_scene.ml.nn.get_layer_text().c_str());
    std::vector<float> dummy(train_scene.ml.nn.get_input_dim().size());
    show_nn(dummy,0);
}

void MainWindow::on_actionSave_Neural_Network_triggered()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(
                this,
                "Save network",work_path,
                "Network files (*.net *.net.gz);;All files (*)");
    if(filename.isEmpty())
        return;
    train_scene.ml.nn.save_to_file<gz_ostream>(filename.toStdString().c_str());
}

void MainWindow::on_load_nn_data_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(
                this,
                "Open data",work_path,
                "Data files (*.bin *.bin.gz);;All files (*)");
    if(filename.isEmpty())
        return;
    if(nn_thread.has_started())
    {
        nn_thread.clear();
        nn_timer->stop();
    }
    nn_data.load_from_file<gz_istream>(filename.toStdString().c_str());
    ui->data_pos->setMaximum(nn_data.size()-1);
}

void MainWindow::on_save_nn_data_clicked()
{
    if(nn_data.empty())
        return;
    QString filename;
    filename = QFileDialog::getSaveFileName(
                this,
                "Save data",work_path,
                "Data files (*.bin *.bin.gz);;All files (*)");
    if(filename.isEmpty())
        return;
    nn_data.save_to_file<gz_ostream>(filename.toStdString().c_str());
}


void MainWindow::on_del_nn_data_clicked()
{
    if(nn_data.empty() || ui->data_pos->value() >= nn_data.size())
        return;
    if(nn_thread.has_started())
    {
        nn_thread.clear();
        nn_timer->stop();
    }
    nn_data.data.erase(nn_data.data.begin() + ui->data_pos->value());
    nn_data.data_label.erase(nn_data.data_label.begin() + ui->data_pos->value());
    ui->data_pos->setMaximum(nn_data.size()-1);
    on_data_pos_valueChanged(0);
}

void MainWindow::on_train_nn_clicked()
{
    if(train_scene.ml.nn.empty() || nn_data.input.size() != train_scene.ml.nn.get_input_size())
    {
        QMessageBox::information(this,"Error","Data does not fit the network layers");
        return;
    }

    if(nn_thread.has_started())
    {
        nn_thread.clear();
        nn_timer->stop();
    }

    train_scene.ml.train.learning_rate = ui->learning_rate->value();
    train_scene.ml.train.momentum = ui->momentum->value();
    train_scene.ml.train.batch_size = ui->batch_size->value();
    train_scene.ml.train.epoch = ui->epoch->value();

    end_training = false;
    test_error = 100;
    training_error = 100;
    nn_thread.run([&]()
    {
        auto on_enumerate_epoch = [&](){
            //aug_data.rotate_permute();
            //test_error = train_scene.ml.nn.test_error(aug_data.data,aug_data.data_label);
            training_error = train_scene.ml.train.get_training_error();
            std::cout << "training error:" << training_error << "  test error:" << test_error << std::endl;
        };
        tipl::ml::network_data_proxy<unsigned char> train_data;
        train_data = nn_data;
        train_scene.ml.train.initialize_training(train_scene.ml.nn);
        train_scene.ml.train.train(train_scene.ml.nn,train_data,nn_thread.terminated, on_enumerate_epoch);
        end_training = true;
    });

    nn_timer.reset(new QTimer(this));
    connect(nn_timer.get(), SIGNAL(timeout()), this, SLOT(on_nn_timer()));
    nn_timer->setInterval(200);
    nn_timer->start();
}

void MainWindow::show_nn(const std::vector<float>& data,unsigned char label)
{
    tipl::color_image I2;
    //train_scene.ml.nn.to_image(I2,data,label,20,std::max<int>(250,ui->nn_view->width()-20));
    QImage qimage((unsigned char*)&*I2.begin(),I2.width(),I2.height(),QImage::Format_RGB32);
    nn_image = qimage.copy();
    nn_scene.setSceneRect(0, 0, nn_image.width(),nn_image.height());
    nn_scene.clear();
    nn_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    nn_scene.addRect(0, 0, nn_image.width(),nn_image.height(),QPen(),nn_image);
}

void MainWindow::on_nn_timer()
{
    ui->training_error_label->setText(QString("%1 %").arg(training_error));
    ui->test_error_label->setText(QString("%1 %").arg(test_error));
    show_nn(nn_data.data[ui->data_pos->value()],nn_data.data_label[ui->data_pos->value()]);
    if(end_training)
        nn_timer->stop();
}



void MainWindow::on_reset_nn_clicked()
{
    if(nn_thread.has_started())
    {
        nn_thread.clear();
        nn_timer->stop();
    }
    train_scene.ml.nn.reset();
    if(!(train_scene.ml.nn << ui->nn->text().toStdString()))
    {
        QMessageBox::information(this,"Error",QString("Invalid network text: %1").arg(train_scene.ml.nn.error_msg.c_str()));
        train_scene.ml.nn.reset();
        return;
    }
    train_scene.ml.nn.init_weights();
    std::vector<float> dummy(train_scene.ml.nn.get_input_dim().size());
    show_nn(dummy,0);
}
void MainWindow::on_clear_nn_clicked()
{
    if(nn_thread.has_started())
    {
        nn_thread.clear();
        nn_timer->stop();
    }
    train_scene.ml.nn.reset();
    std::vector<float> dummy(train_scene.ml.nn.get_input_dim().size());
    show_nn(dummy,0);
}


void MainWindow::on_data_pos_valueChanged(int value)
{
    if(nn_data.empty())
        return;
    auto& data = nn_data.data[ui->data_pos->value()];
    tipl::grayscale_image I(tipl::geometry<2>(nn_data.input[0],nn_data.input[1]*nn_data.input[2]));
    for(int i = 0;i < data.size();++i)
        I[i] = std::floor((data[i]+1.0f)*127.5f);
    tipl::color_image cI = I;
    QImage qimage((unsigned char*)&*cI.begin(),cI.width(),cI.height(),QImage::Format_RGB32);



    data_image = qimage.scaledToWidth(std::max<int>(16,data_scene.views()[0]->width()*0.5-15));
    data_scene.setSceneRect(0, 0, data_image.width()*2,data_image.height()+40);
    data_scene.clear();
    data_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    data_scene.addRect(0, 0, data_image.width(),data_image.height(),QPen(),data_image);

    if(nn_data.input[2] == 3) // RGB
    {
        tipl::color_image I2(tipl::geometry<2>(nn_data.input[0],nn_data.input[1]));
        int shift1 = I2.size();
        int shift2 = shift1 + shift1;
        for(int i = 0;i < I2.size();++i)
            I2[i] = tipl::rgb(I[i+shift2],I[i+shift1],I[i]);
        QImage qimage2((unsigned char*)&*I2.begin(),I2.width(),I2.height(),QImage::Format_RGB32);
        data_image2 = qimage2.scaledToWidth(data_image.width());
        data_scene.addRect(data_image.width(), 0, data_image2.width(),data_image2.height(),QPen(),data_image2);
    }
    QString info;
    info = QString("%1/%2").arg(ui->data_pos->value()+1).
                                     arg(nn_data.size());
    QGraphicsTextItem* text = data_scene.addText(info);
    text->moveBy(0,data_image.height());
    info = QString("Label=%1 ").arg((int)nn_data.data_label[ui->data_pos->value()]);

    if(!train_scene.ml.nn.empty() && nn_data.input.size() == train_scene.ml.nn.get_input_size())
        info += QString("NN:%1").arg(train_scene.ml.nn.predict(nn_data.data[ui->data_pos->value()]));
    QGraphicsTextItem* text2 = data_scene.addText(info);
    text2->moveBy(0,data_image.height()+20);

}

void MainWindow::on_resolve_nn_data_clicked()
{
    for(int i = 0;i < nn_data.size();++i)
        if(train_scene.ml.nn.predict(nn_data.data[i]) != nn_data.data_label[i])
        {
            ui->data_pos->setValue(i);
            return;
        }
}

void MainWindow::on_add_nn_data_clicked(int label)
{
    if(!w.get() || output.empty())
        return;
    if(nn_data.empty())
    {
        if(train_scene.ml.nn.get_input_dim()[0] != 0)
        {
            nn_data.input = train_scene.ml.nn.get_input_dim();
            nn_data.output = train_scene.ml.nn.get_output_dim();
        }
        else
        {
            nn_data.input = tipl::geometry<3>(64,64,1);
            nn_data.output = tipl::geometry<3>(2,1,1);
        }
    }
    for(int row = 0;row < output.size();++row)
    {
        std::vector<float> data;
        if(!w->get_picture(data,output[row][0],output[row][1],nn_data.input[0]))
            return;
        nn_data.data.push_back(std::move(data));
        nn_data.data_label.push_back(label);
    }
    ui->data_pos->setMaximum(nn_data.size()-1);
}

void MainWindow::on_add_negative_nn_data_clicked()
{
    on_add_nn_data_clicked(0);
}

void MainWindow::on_add_positive_nn_data_clicked()
{
    on_add_nn_data_clicked(1);
}


void MainWindow::on_actionBatch_quality_check_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
                           this,
                           "Open WSI",work_path,"WSI files (*.svs *.tif *.vms *.vmu *.scn *.mrxs *.ndpi);;All files (*)");
    if (filenames.isEmpty())
        return;

    std::vector<std::string> result;
    result.push_back("\tstain1\tstain2");
    for(int i = 0;i < filenames.size();++i)
        result.push_back(QFileInfo(filenames[i]).baseName().toStdString());
    begin_prog("reading");
    for(int i = 0;check_prog(i,filenames.size());++i)
    {
        result[i+1] += "\t";
        wsi w;
        if(!w.open(filenames[i].toStdString().c_str()))
        {
            QImage I;
            if(!I.load(filenames[i]))
                continue;
            QImage I2 = I.convertToFormat(QImage::Format_RGB32);
            w.rawI.resize(tipl::geometry<2>(I2.width(),I2.height()));
            std::copy((const unsigned int*)I2.bits(),(const unsigned int*)I2.bits() + w.rawI.size(),
                      w.rawI.begin());
            w.open(0);
        }
        result[i+1] += QString::number(w.color1_code).toStdString();
        result[i+1] += "\t";
        result[i+1] += QString::number(w.color2_code).toStdString();
    }
    std::string output;
    for(int i = 0;i < result.size();++i)
    {
        output += result[i];
        output += "\r\n";
    }
    show_info_dialog("Results",output);
}


void MainWindow::on_actionFlip_X_triggered()
{
    for(int i = 0;i < nn_data.size();++i)
    {
        auto I = tipl::make_image(&nn_data.data[i][0],
                tipl::geometry<3>(nn_data.input[0],nn_data.input[1],nn_data.input[2]));
        tipl::flip_x(I);
    }
    on_data_pos_valueChanged(0);
}

void MainWindow::on_actionFlip_Y_triggered()
{
    for(int i = 0;i < nn_data.size();++i)
    {
        auto I = tipl::make_image(&nn_data.data[i][0],
                tipl::geometry<3>(nn_data.input[0],nn_data.input[1],nn_data.input[2]));
        tipl::flip_y(I);
    }
    on_data_pos_valueChanged(0);
}

void MainWindow::on_actionSwap_XY_triggered()
{
    for(int i = 0;i < nn_data.size();++i)
    {
        auto I = tipl::make_image(&nn_data.data[i][0],
                 tipl::geometry<3>(nn_data.input[0],nn_data.input[1],nn_data.input[2]));
        tipl::swap_xy(I);
    }
    on_data_pos_valueChanged(0);
}


void MainWindow::on_recog_result_itemSelectionChanged()
{
    if(!w.get() || ui->recog_result->currentRow() == -1 || ui->recog_result->currentRow() == -1)
        return;
    int row = ui->recog_result->currentRow();
    if(row >= output.size())
        return;
    int x = output[row][0]-main_scene.width()*w->get_r(main_scene.level)/2;
    int y = output[row][1]-main_scene.height()*w->get_r(main_scene.level)/2;
    main_scene.move_to(x,y);
    map_scene.update();
}

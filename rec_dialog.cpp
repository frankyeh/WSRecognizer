#include <QFileDialog>
#include <QMessageBox>
#include "rec_dialog.hpp"
#include "ui_rec_dialog.h"
#include "wsi.hpp"
#include "libs/gzip_interface.hpp"

rec_dialog::rec_dialog(QWidget *parent,QString workpath_) :
    QDialog(parent),
    ui(new Ui::rec_dialog),
    workpath(workpath_)
{
    ui->setupUi(this);
    ui->log_widget->hide();
}

rec_dialog::~rec_dialog()
{
    delete ui;
}

void rec_dialog::on_open_wsi_data_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
                           this,
                           "Open WSI",workpath,"WSI files (*.svs *.tif *.vms *.vmu *.scn *.mrxs *.ndpi);;All files (*)");
    if (filenames.isEmpty())
        return;
    for(unsigned int index = 0;index < filenames.size();++index)
    {
        new QListWidgetItem(QFileInfo(filenames[index]).baseName(), ui->file_list);
        file_list.append(filenames[index]);
    }
}

void rec_dialog::on_remove_file_clicked()
{
    if (!ui->file_list->currentItem())
        return;
    file_list.erase(file_list.begin()+ui->file_list->row(ui->file_list->currentItem()));
    ui->file_list->takeItem(ui->file_list->row(ui->file_list->currentItem()));
}

void rec_dialog::on_open_model_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                           this,
                           "Open image",workpath,"text files (*.mdl.gz);;All files (*)");
    if (filename.isEmpty())
        return;
    ml.reset(new train_model);
    if(!ml->load_from_file(filename.toLocal8Bit().begin()))
    {
        QMessageBox::information(this,"Error","Invalid file format",0);
        ml.reset(0);
        return;
    }
    model_name = QFileInfo(filename).baseName();
    ui->classification_label->setText(QString("Classification Model: ")+model_name);
    ui->smoothing->setValue(ml->smoothing);
}

void rec_dialog::add_log(QString text)
{
    log.append(text);
}

void rec_dialog::run_thread(train_model* ml_ptr)
{
    log.clear();
    std::auto_ptr<train_model> ml(ml_ptr);
    ml->predict(0);
    for(file_progress = 0;file_progress < file_list.size() && !terminated;++file_progress)
    {
        {
            add_log(QString("Loading files ") + QString::number(file_progress) + "/" + QString::number(file_list.size()));
            if(!wsi::can_open(file_list[file_progress].toLocal8Bit().begin()))
            {
                add_log(QString("Invalid file format:") + file_list[file_progress]);
                continue;
            }
            if(!w.open(file_list[file_progress].toLocal8Bit().begin()))
            {
                add_log(QString("Cannot open file ")+file_list[file_progress]);
                continue;
            }
            add_log(QString("width:") + QString::number(w.dim[0]) + " height:" + QString::number(w.dim[1]));
            add_log("spatial resolution:" + QString::number(w.pixel_size) + " micron");
        }

        w.run(4000,200,ui->thread->value(),ml.get(),&terminated);
        add_log("recognition completed.");
        if(w.result_pos.empty())
        {
            add_log("No target recognized. Process aborted. Please check the training data?");
            continue;
        }

        add_log(QString("A total of ") + QString::number(w.result_pos.size()) + " targets identified");

        std::string output_name = file_list[file_progress].toLocal8Bit().begin();
        output_name += ".";
        output_name += model_name.toLocal8Bit().begin();
        output_name += ".reg.gz";
        add_log(QString("Writing data to file ") + output_name.c_str());

        w.save_recognition_result(output_name.c_str());
    }
    terminated = true;
}

void rec_dialog::show_run_progress()
{
    ui->progressBar1->setValue(file_progress*100/file_list.size());
    ui->progressBar2->setValue(w.progress);
    ui->log->clear();
    ui->log->appendPlainText(log.join("\n"));
    if(terminated)
    {
        disconnect(timer.get(), SIGNAL(timeout()), this, SLOT(show_run_progress()));
        thread->join();
        ui->run->setText("Run");
        ui->close->setEnabled(true);
        thread.reset(0);
    }
    if(file_progress == file_list.size())
    {
        QMessageBox::information(this,"","Finished!",0);
    }
}

void rec_dialog::on_run_clicked()
{
    // stop thread
    if(thread.get())
    {
        terminated = true;
        return;
    }

    if(!ml.get())
    {
        QMessageBox::information(this,"Error","Please assign the classification model",0);
        return;
    }
    if(file_list.empty())
    {
        QMessageBox::information(this,"Error","Please assign the WSI files",0);
        return;
    }
    ml->smoothing = ui->smoothing->value();
    terminated = false;
    thread.reset(new boost::thread(&rec_dialog::run_thread,this,ml.release()));
    timer.reset(new QTimer(this));
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(show_run_progress()));
    timer->start(500);
    ui->run->setText("Stop");
    ui->close->setEnabled(false);
    ui->log_widget->show();
}

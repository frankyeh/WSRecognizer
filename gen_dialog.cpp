#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include "libs/prog_interface_static_link.h"
#include "libs/gzip_interface.hpp"
#include "gen_dialog.hpp"
#include "wsi.hpp"
#include "ui_gen_dialog.h"

extern image::color_image colormap;
gen_dialog::gen_dialog(QWidget *parent,QString workpath_) :
    QDialog(parent),
    ui(new Ui::gen_dialog),
    workpath(workpath_)
{
    ui->setupUi(this);
}

gen_dialog::~gen_dialog()
{
    delete ui;
}

void gen_dialog::on_open_file_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
                           this,
                           "Open recognition result",workpath,"WSI files (*.reg.gz);;WSI files (*.svs *.tif *.vms *.vmu *.scn *.mrxs *.ndpi);;All files (*)");
    if (filenames.isEmpty())
        return;
    for(unsigned int index = 0;index < filenames.size();++index)
    {
        new QListWidgetItem(QFileInfo(filenames[index]).baseName(), ui->file_list);
        file_list.append(filenames[index]);
    }
}

void gen_dialog::on_clear_file_clicked()
{
    if (!ui->file_list->currentItem())
        return;
    file_list.erase(file_list.begin()+ui->file_list->row(ui->file_list->currentItem()));
    ui->file_list->takeItem(ui->file_list->row(ui->file_list->currentItem()));
}

void gen_dialog::on_buttonBox_accepted()
{
    begin_prog("Generate images");
    for(unsigned int index = 0;check_prog(index,file_list.size());++index)
    {
        wsi w;
        if(QFileInfo(file_list[index]).suffix() != "gz") // recognition file
        {
            if(!w.open(file_list[index].toLocal8Bit().begin()))
            {
                QMessageBox::information(this,"Error",file_list[index] + " is an invalid file",0);
                continue;
            }
            if(!w.load_text_reco_result((file_list[index]+".txt").toLocal8Bit().begin()))
            {
                QMessageBox::information(this,"Error",file_list[index] + " has no recognition results",0);
                continue;
            }
        }
        else
        if(!w.load_recognition_result(file_list[index].toLocal8Bit().begin()))
        {
            QMessageBox::information(this,"Error",file_list[index] + " is an invalid file",0);
            continue;
        }
        image::basic_image<float,2> sdi_value;
        image::basic_image<unsigned char,2> sdi_contour;
        w.get_distribution_image(sdi_value,sdi_contour,
                                 ui->resolution->value(),ui->resolution->value(),
                                 ui->type->currentIndex(),ui->min_size->value(),ui->max_size->value());

        image::color_image sdi_image(sdi_value.geometry());

        {
            float r = 255.99/(ui->value_max->value()-ui->value_min->value());
            for(unsigned int index = 0;index < sdi_value.size();++index)
            {
                int i = std::max<int>(0,std::min<int>(255,std::floor((sdi_value[index]-ui->value_min->value())*r)));
                sdi_image[index] = colormap[i];
            }
        }
        if(ui->contour->isChecked())
        {
            for(unsigned int index = 0;index < sdi_value.size();++index)
                if(sdi_contour[index])
                    sdi_value[index] = 0;
        }

        if(ui->format->currentIndex() == 1)// mat
        {
            QString output_file = file_list[index] + ".mat";
            image::io::mat_write mat(output_file.toLocal8Bit().begin());
            mat << sdi_value;
        }
        if(ui->format->currentIndex() == 0)// tif
        {
            QString output_file = file_list[index] + ".jpg";
            QImage I((unsigned char*)&*sdi_image.begin(),sdi_image.width(),sdi_image.height(),QImage::Format_RGB32);
            I.save(output_file);
        }
        if(w.is_tma)
        {
            QString output_file = file_list[index] + ".tma.txt";
            w.save_tma_result(output_file.toStdString().c_str());
        }
    }
}

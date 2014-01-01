#include <QFileDialog>
#include <QMessageBox>
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
                           "Open recognition result",workpath,"WSI files (*.reg.gz);;All files (*)");
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
    set_title("Generate images");
    for(unsigned int index = 0;check_prog(index,file_list.size());++index)
    {
        wsi w;
        gz_mat_read mat;
        if(!mat.load_from_file(file_list[index].toLocal8Bit().begin()))
        {
            QMessageBox::information(this,"Error",QString("Failed to open file ") + file_list[index],0);
            continue;
        }

        {
            unsigned int rows,cols,cols2;
            const int* dim = 0;
            const float* x = 0;
            const float* y = 0;
            const float* pixel_size = 0;
            const float* length = 0;
            if(!mat.read("dimension",rows,cols2,dim)||
               !mat.read("pixel_size",rows,cols2,pixel_size) ||
               !mat.read("x",rows,cols,x) ||
               !mat.read("y",rows,cols,y))
            {
                QMessageBox::information(this,"Error",QString("Invalid file format") + file_list[index],0);
                continue;
            }
            mat.read("length",rows,cols,length);
            w.dim[0] = dim[0];
            w.dim[1] = dim[1];
            w.pixel_size = *pixel_size;
            w.result_pos.resize(cols);
            w.result_features.resize(cols);
            for(unsigned int index = 0;index < cols;++index)
            {
                w.result_pos[index][0] = x[index];
                w.result_pos[index][1] = y[index];
                if(length)
                    w.result_features[index] = length[index];
            }
        }

        image::basic_image<float,2> sdi_value;
        w.get_distribution_image(sdi_value,
                                 ui->resolution->value(),ui->resolution->value(),ui->type->currentIndex());

        image::color_image sdi_image(sdi_value.geometry());

        {
            float r = 255.99/(ui->value_max->value()-ui->value_min->value());
            for(unsigned int index = 0;index < sdi_value.size();++index)
            {
                int i = std::max<int>(0,std::min<int>(255,std::floor((sdi_value[index]-ui->value_min->value())*r)));
                sdi_image[index] = colormap[i];
            }
        }

        if(ui->format->currentIndex() == 1)// mat
        {
            QString output_file = file_list[index] + ".mat";
            image::io::mat_write mat(output_file.toLocal8Bit().begin());
            mat << sdi_value;
            continue;
        }
        if(ui->format->currentIndex() == 0)// tif
        {
            QString output_file = file_list[index] + ".tif";
            QImage I((unsigned char*)&*sdi_image.begin(),sdi_image.width(),sdi_image.height(),QImage::Format_RGB32);
            I.save(output_file);
            continue;
        }
    }
}

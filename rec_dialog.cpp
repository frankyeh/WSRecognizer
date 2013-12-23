#include "rec_dialog.hpp"
#include "ui_rec_dialog.h"

rec_dialog::rec_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rec_dialog)
{
    ui->setupUi(this);
}

rec_dialog::~rec_dialog()
{
    delete ui;
}

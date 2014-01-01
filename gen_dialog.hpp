#ifndef GEN_DIALOG_HPP
#define GEN_DIALOG_HPP

#include <QDialog>

namespace Ui {
class gen_dialog;
}

class gen_dialog : public QDialog
{
    Q_OBJECT
private:
    QString workpath;
    QStringList file_list;
public:

    explicit gen_dialog(QWidget *parent,QString workpath_);
    ~gen_dialog();
    
private slots:
    void on_open_file_clicked();

    void on_clear_file_clicked();

    void on_buttonBox_accepted();

private:
    Ui::gen_dialog *ui;
};

#endif // GEN_DIALOG_HPP

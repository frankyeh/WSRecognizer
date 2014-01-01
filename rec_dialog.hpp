#ifndef REC_DIALOG_HPP
#define REC_DIALOG_HPP
#include "boost/thread.hpp"
#include <QDialog>
#include <QTimer>
#include "train_model.hpp"
#include "wsi.hpp"
namespace Ui {
class rec_dialog;
}

class rec_dialog : public QDialog
{
    Q_OBJECT
private:
    QString workpath,model_name;
    QStringList file_list;
private: 
    std::auto_ptr<train_model> ml;
    wsi w;
private:
    unsigned int file_progress;
    std::auto_ptr<boost::thread> thread;
    std::auto_ptr<QTimer> timer;
    bool terminated;
    QStringList log;
    void run_thread(train_model* ml_ptr);
public:
    explicit rec_dialog(QWidget *parent,QString workpath_);
    ~rec_dialog();
    void add_log(QString text);
private slots:
    void on_open_wsi_data_clicked();

    void on_remove_file_clicked();

    void on_open_model_file_clicked();

    void show_run_progress();
    void on_run_clicked();

private:
    Ui::rec_dialog *ui;
};

#endif // REC_DIALOG_HPP

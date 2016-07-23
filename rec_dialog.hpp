#ifndef REC_DIALOG_HPP
#define REC_DIALOG_HPP
#include <future>
#include <QtWidgets/QDialog>
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
    wsi w;
private:
    unsigned int file_progress;
    std::future<void> future;
    std::auto_ptr<QTimer> timer;
    bool terminated;
    QStringList log;
    void run_thread(void);
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

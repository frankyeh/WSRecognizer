#ifndef REC_DIALOG_HPP
#define REC_DIALOG_HPP

#include <QDialog>

namespace Ui {
class rec_dialog;
}

class rec_dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit rec_dialog(QWidget *parent = 0);
    ~rec_dialog();
    
private:
    Ui::rec_dialog *ui;
};

#endif // REC_DIALOG_HPP

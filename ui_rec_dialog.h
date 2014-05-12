/********************************************************************************
** Form generated from reading UI file 'rec_dialog.ui'
**
** Created: Thu Mar 6 12:45:05 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REC_DIALOG_H
#define UI_REC_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_rec_dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *classification_label;
    QToolButton *open_model_file;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer;
    QToolButton *remove_file;
    QToolButton *open_wsi_data;
    QListWidget *file_list;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_11;
    QSpinBox *smoothing;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_6;
    QSpinBox *thread;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *run;
    QPushButton *close;
    QWidget *log_widget;
    QVBoxLayout *verticalLayout_3;
    QProgressBar *progressBar1;
    QProgressBar *progressBar2;
    QPlainTextEdit *log;

    void setupUi(QDialog *rec_dialog)
    {
        if (rec_dialog->objectName().isEmpty())
            rec_dialog->setObjectName(QString::fromUtf8("rec_dialog"));
        rec_dialog->resize(370, 404);
        verticalLayout = new QVBoxLayout(rec_dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        groupBox = new QGroupBox(rec_dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        classification_label = new QLabel(groupBox);
        classification_label->setObjectName(QString::fromUtf8("classification_label"));

        horizontalLayout->addWidget(classification_label);

        open_model_file = new QToolButton(groupBox);
        open_model_file->setObjectName(QString::fromUtf8("open_model_file"));
        open_model_file->setMaximumSize(QSize(23, 22));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/open.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        open_model_file->setIcon(icon);

        horizontalLayout->addWidget(open_model_file);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_3->addWidget(label_8);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        remove_file = new QToolButton(groupBox);
        remove_file->setObjectName(QString::fromUtf8("remove_file"));
        remove_file->setMaximumSize(QSize(23, 22));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/delete.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        remove_file->setIcon(icon1);

        horizontalLayout_3->addWidget(remove_file);

        open_wsi_data = new QToolButton(groupBox);
        open_wsi_data->setObjectName(QString::fromUtf8("open_wsi_data"));
        open_wsi_data->setMaximumSize(QSize(23, 22));
        open_wsi_data->setIcon(icon);

        horizontalLayout_3->addWidget(open_wsi_data);


        verticalLayout_2->addLayout(horizontalLayout_3);

        file_list = new QListWidget(groupBox);
        file_list->setObjectName(QString::fromUtf8("file_list"));

        verticalLayout_2->addWidget(file_list);


        horizontalLayout_7->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_11 = new QLabel(rec_dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_8->addWidget(label_11);

        smoothing = new QSpinBox(rec_dialog);
        smoothing->setObjectName(QString::fromUtf8("smoothing"));
        smoothing->setMaximum(10);
        smoothing->setSingleStep(1);
        smoothing->setValue(5);

        horizontalLayout_8->addWidget(smoothing);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        label_6 = new QLabel(rec_dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_5->addWidget(label_6);

        thread = new QSpinBox(rec_dialog);
        thread->setObjectName(QString::fromUtf8("thread"));
        thread->setMinimum(1);
        thread->setMaximum(12);
        thread->setValue(4);

        horizontalLayout_5->addWidget(thread);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        run = new QPushButton(rec_dialog);
        run->setObjectName(QString::fromUtf8("run"));

        horizontalLayout_2->addWidget(run);

        close = new QPushButton(rec_dialog);
        close->setObjectName(QString::fromUtf8("close"));

        horizontalLayout_2->addWidget(close);


        verticalLayout->addLayout(horizontalLayout_2);

        log_widget = new QWidget(rec_dialog);
        log_widget->setObjectName(QString::fromUtf8("log_widget"));
        verticalLayout_3 = new QVBoxLayout(log_widget);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        progressBar1 = new QProgressBar(log_widget);
        progressBar1->setObjectName(QString::fromUtf8("progressBar1"));
        progressBar1->setValue(0);

        verticalLayout_3->addWidget(progressBar1);

        progressBar2 = new QProgressBar(log_widget);
        progressBar2->setObjectName(QString::fromUtf8("progressBar2"));
        progressBar2->setValue(0);

        verticalLayout_3->addWidget(progressBar2);

        log = new QPlainTextEdit(log_widget);
        log->setObjectName(QString::fromUtf8("log"));

        verticalLayout_3->addWidget(log);


        verticalLayout->addWidget(log_widget);


        retranslateUi(rec_dialog);
        QObject::connect(close, SIGNAL(clicked()), rec_dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(rec_dialog);
    } // setupUi

    void retranslateUi(QDialog *rec_dialog)
    {
        rec_dialog->setWindowTitle(QApplication::translate("rec_dialog", "Batch whole slide recognition", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("rec_dialog", "Image Data", 0, QApplication::UnicodeUTF8));
        classification_label->setText(QApplication::translate("rec_dialog", "Classification Model", 0, QApplication::UnicodeUTF8));
        open_model_file->setText(QString());
        label_8->setText(QApplication::translate("rec_dialog", "WSI Files", 0, QApplication::UnicodeUTF8));
        remove_file->setText(QApplication::translate("rec_dialog", "...", 0, QApplication::UnicodeUTF8));
        open_wsi_data->setText(QString());
        label_11->setText(QApplication::translate("rec_dialog", "Smoothing:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("rec_dialog", "Threads:", 0, QApplication::UnicodeUTF8));
        run->setText(QApplication::translate("rec_dialog", "Run", 0, QApplication::UnicodeUTF8));
        close->setText(QApplication::translate("rec_dialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class rec_dialog: public Ui_rec_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REC_DIALOG_H

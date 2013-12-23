/********************************************************************************
** Form generated from reading UI file 'rec_dialog.ui'
**
** Created: Mon Dec 23 00:14:05 2013
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
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_rec_dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QToolButton *toolButton_3;
    QListView *listView;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QToolButton *toolButton_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *rec_dialog)
    {
        if (rec_dialog->objectName().isEmpty())
            rec_dialog->setObjectName(QString::fromUtf8("rec_dialog"));
        rec_dialog->resize(406, 348);
        verticalLayout = new QVBoxLayout(rec_dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        toolButton_3 = new QToolButton(rec_dialog);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));

        horizontalLayout_3->addWidget(toolButton_3);


        verticalLayout->addLayout(horizontalLayout_3);

        listView = new QListView(rec_dialog);
        listView->setObjectName(QString::fromUtf8("listView"));

        verticalLayout->addWidget(listView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(rec_dialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(rec_dialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        toolButton = new QToolButton(rec_dialog);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        horizontalLayout->addWidget(toolButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(rec_dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(rec_dialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);

        toolButton_2 = new QToolButton(rec_dialog);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        horizontalLayout_2->addWidget(toolButton_2);


        verticalLayout->addLayout(horizontalLayout_2);

        buttonBox = new QDialogButtonBox(rec_dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(rec_dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), rec_dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), rec_dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(rec_dialog);
    } // setupUi

    void retranslateUi(QDialog *rec_dialog)
    {
        rec_dialog->setWindowTitle(QApplication::translate("rec_dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        toolButton_3->setText(QApplication::translate("rec_dialog", "...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("rec_dialog", "Foregound", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("rec_dialog", "...", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("rec_dialog", "Background", 0, QApplication::UnicodeUTF8));
        toolButton_2->setText(QApplication::translate("rec_dialog", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class rec_dialog: public Ui_rec_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REC_DIALOG_H

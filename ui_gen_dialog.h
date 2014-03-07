/********************************************************************************
** Form generated from reading UI file 'gen_dialog.ui'
**
** Created: Fri Jan 10 15:57:45 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GEN_DIALOG_H
#define UI_GEN_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gen_dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QToolButton *clear_file;
    QToolButton *open_file;
    QListWidget *file_list;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *type;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *format;
    QWidget *tif_widget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_3;
    QDoubleSpinBox *value_min;
    QDoubleSpinBox *value_max;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;
    QSpinBox *resolution;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QLabel *label_8;
    QSpinBox *min_size;
    QSpinBox *max_size;
    QLabel *label_14;
    QCheckBox *contour;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *gen_dialog)
    {
        if (gen_dialog->objectName().isEmpty())
            gen_dialog->setObjectName(QString::fromUtf8("gen_dialog"));
        gen_dialog->resize(293, 326);
        verticalLayout = new QVBoxLayout(gen_dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_6 = new QLabel(gen_dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        clear_file = new QToolButton(gen_dialog);
        clear_file->setObjectName(QString::fromUtf8("clear_file"));
        clear_file->setMaximumSize(QSize(23, 22));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/delete.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        clear_file->setIcon(icon);

        horizontalLayout->addWidget(clear_file);

        open_file = new QToolButton(gen_dialog);
        open_file->setObjectName(QString::fromUtf8("open_file"));
        open_file->setMaximumSize(QSize(23, 22));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/open.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        open_file->setIcon(icon1);

        horizontalLayout->addWidget(open_file);


        verticalLayout->addLayout(horizontalLayout);

        file_list = new QListWidget(gen_dialog);
        file_list->setObjectName(QString::fromUtf8("file_list"));

        verticalLayout->addWidget(file_list);

        groupBox_2 = new QGroupBox(gen_dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        type = new QComboBox(groupBox_2);
        type->setObjectName(QString::fromUtf8("type"));

        horizontalLayout_3->addWidget(type);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        format = new QComboBox(groupBox_2);
        format->setObjectName(QString::fromUtf8("format"));

        horizontalLayout_2->addWidget(format);


        verticalLayout_3->addLayout(horizontalLayout_2);

        tif_widget = new QWidget(groupBox_2);
        tif_widget->setObjectName(QString::fromUtf8("tif_widget"));
        verticalLayout_2 = new QVBoxLayout(tif_widget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);

        label_3 = new QLabel(tif_widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_9->addWidget(label_3);

        value_min = new QDoubleSpinBox(tif_widget);
        value_min->setObjectName(QString::fromUtf8("value_min"));

        horizontalLayout_9->addWidget(value_min);

        value_max = new QDoubleSpinBox(tif_widget);
        value_max->setObjectName(QString::fromUtf8("value_max"));
        value_max->setMaximum(100);
        value_max->setSingleStep(5);
        value_max->setValue(5);

        horizontalLayout_9->addWidget(value_max);


        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_2);

        label_4 = new QLabel(tif_widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_10->addWidget(label_4);

        resolution = new QSpinBox(tif_widget);
        resolution->setObjectName(QString::fromUtf8("resolution"));
        resolution->setMinimum(5);
        resolution->setMaximum(200);
        resolution->setSingleStep(5);
        resolution->setValue(40);

        horizontalLayout_10->addWidget(resolution);

        label_7 = new QLabel(tif_widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_10->addWidget(label_7);


        verticalLayout_2->addLayout(horizontalLayout_10);


        verticalLayout_3->addWidget(tif_widget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_4->addWidget(label_8);

        min_size = new QSpinBox(groupBox_2);
        min_size->setObjectName(QString::fromUtf8("min_size"));
        min_size->setMaximum(50);
        min_size->setSingleStep(5);

        horizontalLayout_4->addWidget(min_size);

        max_size = new QSpinBox(groupBox_2);
        max_size->setObjectName(QString::fromUtf8("max_size"));
        max_size->setMaximum(1000);
        max_size->setSingleStep(5);
        max_size->setValue(100);

        horizontalLayout_4->addWidget(max_size);

        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_4->addWidget(label_14);


        verticalLayout_3->addLayout(horizontalLayout_4);

        contour = new QCheckBox(groupBox_2);
        contour->setObjectName(QString::fromUtf8("contour"));

        verticalLayout_3->addWidget(contour);


        verticalLayout->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(gen_dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(gen_dialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), gen_dialog, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), gen_dialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(gen_dialog);
    } // setupUi

    void retranslateUi(QDialog *gen_dialog)
    {
        gen_dialog->setWindowTitle(QApplication::translate("gen_dialog", "Batch generate images", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("gen_dialog", "File list", 0, QApplication::UnicodeUTF8));
        clear_file->setText(QApplication::translate("gen_dialog", "...", 0, QApplication::UnicodeUTF8));
        open_file->setText(QApplication::translate("gen_dialog", "...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("gen_dialog", "Output", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("gen_dialog", "Type", 0, QApplication::UnicodeUTF8));
        type->clear();
        type->insertItems(0, QStringList()
         << QApplication::translate("gen_dialog", "Stain density imaging", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("gen_dialog", "Stain size imaging", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("gen_dialog", "Format", 0, QApplication::UnicodeUTF8));
        format->clear();
        format->insertItems(0, QStringList()
         << QApplication::translate("gen_dialog", "TIF file", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("gen_dialog", "MAT file", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("gen_dialog", "min/max value", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("gen_dialog", "resolution", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("gen_dialog", "microns", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("gen_dialog", "Size min/max size", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("gen_dialog", "um", 0, QApplication::UnicodeUTF8));
        contour->setText(QApplication::translate("gen_dialog", "Add contour", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class gen_dialog: public Ui_gen_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GEN_DIALOG_H

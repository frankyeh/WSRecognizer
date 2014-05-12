/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun May 11 23:00:11 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "plot/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *actionBatch_analysis;
    QAction *actionBatch_generate_images;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    QWidget *widget;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *show_recog;
    QCheckBox *result_edge;
    QSpacerItem *horizontalSpacer_2;
    QGraphicsView *main_view;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_14;
    QSlider *map_size;
    QGraphicsView *graphicsView;
    QVBoxLayout *verticalLayout;
    QCheckBox *show_map_mask;
    QPushButton *dilation;
    QPushButton *erosion;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QGraphicsView *info_view;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *info_widget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuRecent_Files;
    QMenu *menuView;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget_2;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *train_view;
    QVBoxLayout *verticalLayout_11;
    QPushButton *save_model;
    QPushButton *open_model;
    QPushButton *new_model;
    QSpacerItem *verticalSpacer_2;
    QPushButton *recognize_stains;
    QHBoxLayout *horizontalLayout_3;
    QProgressBar *progressBar;
    QPushButton *run;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_8;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_5;
    QSpinBox *smoothing;
    QLabel *label_12;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer;
    QSpinBox *min_size;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_4;
    QSpinBox *max_size;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label;
    QSpacerItem *horizontalSpacer_8;
    QSpinBox *thread_count;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_6;
    QSpinBox *resolution;
    QLabel *label_5;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_13;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_14;
    QComboBox *type;
    QTabWidget *tabWidget_3;
    QWidget *tab_6;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *open_reco_result;
    QPushButton *save_reco_result;
    QCheckBox *contour;
    QCheckBox *flip;
    QLabel *label_3;
    QSlider *orientation;
    QGraphicsView *result_view;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_2;
    QDoubleSpinBox *color_min;
    QDoubleSpinBox *color_max;
    QSpacerItem *horizontalSpacer_9;
    QWidget *tab_7;
    QHBoxLayout *horizontalLayout_5;
    QSplitter *splitter_2;
    QTableWidget *tableWidget;
    QCustomPlot *boxplot;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(880, 530);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/open.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon);
        actionBatch_analysis = new QAction(MainWindow);
        actionBatch_analysis->setObjectName(QString::fromUtf8("actionBatch_analysis"));
        actionBatch_generate_images = new QAction(MainWindow);
        actionBatch_generate_images->setObjectName(QString::fromUtf8("actionBatch_generate_images"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy1);
        splitter->setOrientation(Qt::Vertical);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy2);
        verticalLayout_7 = new QVBoxLayout(widget);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        show_recog = new QCheckBox(widget);
        show_recog->setObjectName(QString::fromUtf8("show_recog"));

        horizontalLayout_2->addWidget(show_recog);

        result_edge = new QCheckBox(widget);
        result_edge->setObjectName(QString::fromUtf8("result_edge"));

        horizontalLayout_2->addWidget(result_edge);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_7->addLayout(horizontalLayout_2);

        main_view = new QGraphicsView(widget);
        main_view->setObjectName(QString::fromUtf8("main_view"));

        verticalLayout_7->addWidget(main_view);

        splitter->addWidget(widget);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_14 = new QHBoxLayout(tab);
        horizontalLayout_14->setSpacing(0);
        horizontalLayout_14->setContentsMargins(6, 6, 6, 6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        map_size = new QSlider(tab);
        map_size->setObjectName(QString::fromUtf8("map_size"));
        map_size->setMinimum(10);
        map_size->setMaximum(100);
        map_size->setValue(40);
        map_size->setOrientation(Qt::Vertical);
        map_size->setInvertedAppearance(true);

        horizontalLayout_14->addWidget(map_size);

        graphicsView = new QGraphicsView(tab);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout_14->addWidget(graphicsView);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        show_map_mask = new QCheckBox(tab);
        show_map_mask->setObjectName(QString::fromUtf8("show_map_mask"));
        show_map_mask->setChecked(true);

        verticalLayout->addWidget(show_map_mask);

        dilation = new QPushButton(tab);
        dilation->setObjectName(QString::fromUtf8("dilation"));
        dilation->setMaximumSize(QSize(65535, 22));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/up.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        dilation->setIcon(icon1);

        verticalLayout->addWidget(dilation);

        erosion = new QPushButton(tab);
        erosion->setObjectName(QString::fromUtf8("erosion"));
        erosion->setMaximumSize(QSize(65535, 22));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/down.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        erosion->setIcon(icon2);

        verticalLayout->addWidget(erosion);


        horizontalLayout_14->addLayout(verticalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(6, 6, 6, 6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        info_view = new QGraphicsView(tab_2);
        info_view->setObjectName(QString::fromUtf8("info_view"));

        verticalLayout_5->addWidget(info_view);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        info_widget = new QTableWidget(tab_3);
        if (info_widget->columnCount() < 2)
            info_widget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        info_widget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        info_widget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        info_widget->setObjectName(QString::fromUtf8("info_widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(info_widget->sizePolicy().hasHeightForWidth());
        info_widget->setSizePolicy(sizePolicy3);

        verticalLayout_4->addWidget(info_widget);

        tabWidget->addTab(tab_3, QString());
        splitter->addWidget(tabWidget);

        verticalLayout_2->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 880, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuRecent_Files = new QMenu(menu_File);
        menuRecent_Files->setObjectName(QString::fromUtf8("menuRecent_Files"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QString::fromUtf8("dockWidget_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(dockWidget_2->sizePolicy().hasHeightForWidth());
        dockWidget_2->setSizePolicy(sizePolicy4);
        dockWidget_2->setMinimumSize(QSize(235, 215));
        dockWidget_2->setMaximumSize(QSize(524287, 250));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget_2 = new QTabWidget(dockWidgetContents_2);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        sizePolicy3.setHeightForWidth(tabWidget_2->sizePolicy().hasHeightForWidth());
        tabWidget_2->setSizePolicy(sizePolicy3);
        tabWidget_2->setMaximumSize(QSize(16777215, 300));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_9 = new QVBoxLayout(tab_4);
        verticalLayout_9->setSpacing(3);
        verticalLayout_9->setContentsMargins(6, 6, 6, 6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        groupBox = new QGroupBox(tab_4);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy5);
        groupBox->setMaximumSize(QSize(16777215, 150));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        train_view = new QGraphicsView(groupBox);
        train_view->setObjectName(QString::fromUtf8("train_view"));
        sizePolicy3.setHeightForWidth(train_view->sizePolicy().hasHeightForWidth());
        train_view->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(train_view);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        save_model = new QPushButton(groupBox);
        save_model->setObjectName(QString::fromUtf8("save_model"));
        save_model->setMaximumSize(QSize(70, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/save.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        save_model->setIcon(icon3);

        verticalLayout_11->addWidget(save_model);

        open_model = new QPushButton(groupBox);
        open_model->setObjectName(QString::fromUtf8("open_model"));
        open_model->setMaximumSize(QSize(70, 24));
        open_model->setIcon(icon);

        verticalLayout_11->addWidget(open_model);

        new_model = new QPushButton(groupBox);
        new_model->setObjectName(QString::fromUtf8("new_model"));
        sizePolicy3.setHeightForWidth(new_model->sizePolicy().hasHeightForWidth());
        new_model->setSizePolicy(sizePolicy3);
        new_model->setMaximumSize(QSize(70, 24));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/delete.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        new_model->setIcon(icon4);

        verticalLayout_11->addWidget(new_model);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_11->addItem(verticalSpacer_2);

        recognize_stains = new QPushButton(groupBox);
        recognize_stains->setObjectName(QString::fromUtf8("recognize_stains"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(recognize_stains->sizePolicy().hasHeightForWidth());
        recognize_stains->setSizePolicy(sizePolicy6);
        recognize_stains->setMinimumSize(QSize(0, 0));
        recognize_stains->setMaximumSize(QSize(70, 24));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/run.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        recognize_stains->setIcon(icon5);

        verticalLayout_11->addWidget(recognize_stains);


        horizontalLayout->addLayout(verticalLayout_11);


        verticalLayout_9->addWidget(groupBox);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        progressBar = new QProgressBar(tab_4);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 24));
        progressBar->setValue(0);

        horizontalLayout_3->addWidget(progressBar);

        run = new QPushButton(tab_4);
        run->setObjectName(QString::fromUtf8("run"));
        sizePolicy6.setHeightForWidth(run->sizePolicy().hasHeightForWidth());
        run->setSizePolicy(sizePolicy6);
        run->setMaximumSize(QSize(16777215, 24));
        run->setIcon(icon5);

        horizontalLayout_3->addWidget(run);


        verticalLayout_9->addLayout(horizontalLayout_3);

        tabWidget_2->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        verticalLayout_8 = new QVBoxLayout(tab_5);
        verticalLayout_8->setSpacing(3);
        verticalLayout_8->setContentsMargins(6, 6, 6, 6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        scrollArea = new QScrollArea(tab_5);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 235, 200));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        groupBox_4 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_10 = new QVBoxLayout(groupBox_4);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_7->addWidget(label_11);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        smoothing = new QSpinBox(groupBox_4);
        smoothing->setObjectName(QString::fromUtf8("smoothing"));
        smoothing->setMaximum(10);
        smoothing->setSingleStep(1);
        smoothing->setValue(5);

        horizontalLayout_7->addWidget(smoothing);

        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_7->addWidget(label_12);


        verticalLayout_10->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_4->addWidget(label_6);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        min_size = new QSpinBox(groupBox_4);
        min_size->setObjectName(QString::fromUtf8("min_size"));
        min_size->setMaximum(50);
        min_size->setSingleStep(5);

        horizontalLayout_4->addWidget(min_size);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_4->addWidget(label_8);


        verticalLayout_10->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_6->addWidget(label_7);

        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_6->addWidget(label_10);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        max_size = new QSpinBox(groupBox_4);
        max_size->setObjectName(QString::fromUtf8("max_size"));
        max_size->setMaximum(1000);
        max_size->setSingleStep(5);
        max_size->setValue(100);

        horizontalLayout_6->addWidget(max_size);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_6->addWidget(label_9);


        verticalLayout_10->addLayout(horizontalLayout_6);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(0);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label = new QLabel(groupBox_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_11->addWidget(label);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_8);

        thread_count = new QSpinBox(groupBox_4);
        thread_count->setObjectName(QString::fromUtf8("thread_count"));
        thread_count->setMaximumSize(QSize(16777215, 24));
        thread_count->setMinimum(1);
        thread_count->setMaximum(10);
        thread_count->setValue(4);

        horizontalLayout_11->addWidget(thread_count);


        verticalLayout_10->addLayout(horizontalLayout_11);


        verticalLayout_6->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_12 = new QVBoxLayout(groupBox_5);
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_10->addWidget(label_4);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);

        resolution = new QSpinBox(groupBox_5);
        resolution->setObjectName(QString::fromUtf8("resolution"));
        resolution->setMaximumSize(QSize(16777215, 24));
        resolution->setMinimum(10);
        resolution->setMaximum(100);
        resolution->setSingleStep(10);
        resolution->setValue(40);

        horizontalLayout_10->addWidget(resolution);

        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_10->addWidget(label_5);


        verticalLayout_12->addLayout(horizontalLayout_10);


        verticalLayout_6->addWidget(groupBox_5);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_8->addWidget(scrollArea);

        tabWidget_2->addTab(tab_5, QString());

        verticalLayout_3->addWidget(tabWidget_2);

        dockWidget_2->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_2);
        dockWidget_3 = new QDockWidget(MainWindow);
        dockWidget_3->setObjectName(QString::fromUtf8("dockWidget_3"));
        sizePolicy5.setHeightForWidth(dockWidget_3->sizePolicy().hasHeightForWidth());
        dockWidget_3->setSizePolicy(sizePolicy5);
        dockWidget_3->setMinimumSize(QSize(312, 242));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        verticalLayout_13 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_13->setSpacing(3);
        verticalLayout_13->setContentsMargins(6, 6, 6, 6);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_14 = new QLabel(dockWidgetContents_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_9->addWidget(label_14);

        type = new QComboBox(dockWidgetContents_3);
        type->setObjectName(QString::fromUtf8("type"));
        type->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_9->addWidget(type);


        verticalLayout_13->addLayout(horizontalLayout_9);

        tabWidget_3 = new QTabWidget(dockWidgetContents_3);
        tabWidget_3->setObjectName(QString::fromUtf8("tabWidget_3"));
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        verticalLayout_14 = new QVBoxLayout(tab_6);
        verticalLayout_14->setSpacing(3);
        verticalLayout_14->setContentsMargins(3, 3, 3, 3);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        open_reco_result = new QPushButton(tab_6);
        open_reco_result->setObjectName(QString::fromUtf8("open_reco_result"));
        open_reco_result->setMaximumSize(QSize(60, 24));
        open_reco_result->setIcon(icon);

        horizontalLayout_8->addWidget(open_reco_result);

        save_reco_result = new QPushButton(tab_6);
        save_reco_result->setObjectName(QString::fromUtf8("save_reco_result"));
        save_reco_result->setMaximumSize(QSize(60, 24));
        save_reco_result->setIcon(icon3);

        horizontalLayout_8->addWidget(save_reco_result);

        contour = new QCheckBox(tab_6);
        contour->setObjectName(QString::fromUtf8("contour"));
        contour->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_8->addWidget(contour);

        flip = new QCheckBox(tab_6);
        flip->setObjectName(QString::fromUtf8("flip"));

        horizontalLayout_8->addWidget(flip);

        label_3 = new QLabel(tab_6);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_8->addWidget(label_3);

        orientation = new QSlider(tab_6);
        orientation->setObjectName(QString::fromUtf8("orientation"));
        orientation->setMaximum(36);
        orientation->setPageStep(6);
        orientation->setOrientation(Qt::Horizontal);

        horizontalLayout_8->addWidget(orientation);


        verticalLayout_14->addLayout(horizontalLayout_8);

        result_view = new QGraphicsView(tab_6);
        result_view->setObjectName(QString::fromUtf8("result_view"));

        verticalLayout_14->addWidget(result_view);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_2 = new QLabel(tab_6);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_12->addWidget(label_2);

        color_min = new QDoubleSpinBox(tab_6);
        color_min->setObjectName(QString::fromUtf8("color_min"));
        color_min->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_12->addWidget(color_min);

        color_max = new QDoubleSpinBox(tab_6);
        color_max->setObjectName(QString::fromUtf8("color_max"));
        color_max->setMaximumSize(QSize(16777215, 24));
        color_max->setDecimals(1);
        color_max->setMaximum(50);
        color_max->setValue(5);

        horizontalLayout_12->addWidget(color_max);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);


        verticalLayout_14->addLayout(horizontalLayout_12);

        tabWidget_3->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        horizontalLayout_5 = new QHBoxLayout(tab_7);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        splitter_2 = new QSplitter(tab_7);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        tableWidget = new QTableWidget(splitter_2);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        splitter_2->addWidget(tableWidget);
        boxplot = new QCustomPlot(splitter_2);
        boxplot->setObjectName(QString::fromUtf8("boxplot"));
        boxplot->setMinimumSize(QSize(50, 0));
        splitter_2->addWidget(boxplot);

        horizontalLayout_5->addWidget(splitter_2);

        tabWidget_3->addTab(tab_7, QString());

        verticalLayout_13->addWidget(tabWidget_3);

        dockWidget_3->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_3);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuView->menuAction());
        menu_File->addAction(action_Open);
        menu_File->addSeparator();
        menu_File->addAction(menuRecent_Files->menuAction());
        menu_File->addSeparator();
        menu_File->addAction(actionBatch_analysis);
        menu_File->addAction(actionBatch_generate_images);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        tabWidget_3->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        action_Open->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
        actionBatch_analysis->setText(QApplication::translate("MainWindow", "Batch analysis...", 0, QApplication::UnicodeUTF8));
        actionBatch_generate_images->setText(QApplication::translate("MainWindow", "Batch generate images...", 0, QApplication::UnicodeUTF8));
        show_recog->setText(QApplication::translate("MainWindow", "Show recognition", 0, QApplication::UnicodeUTF8));
        result_edge->setText(QApplication::translate("MainWindow", "Edge", 0, QApplication::UnicodeUTF8));
        show_map_mask->setText(QApplication::translate("MainWindow", "Show mask", 0, QApplication::UnicodeUTF8));
        dilation->setText(QApplication::translate("MainWindow", "Larger", 0, QApplication::UnicodeUTF8));
        erosion->setText(QApplication::translate("MainWindow", "Smaller", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Navigation window", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = info_widget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Property", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = info_widget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Info", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuRecent_Files->setTitle(QApplication::translate("MainWindow", "Recent Files", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        dockWidget_2->setWindowTitle(QApplication::translate("MainWindow", "Recognition window", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Classifier", 0, QApplication::UnicodeUTF8));
        save_model->setText(QApplication::translate("MainWindow", "Save...", 0, QApplication::UnicodeUTF8));
        open_model->setText(QApplication::translate("MainWindow", "Open...", 0, QApplication::UnicodeUTF8));
        new_model->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
        recognize_stains->setText(QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8));
        run->setText(QApplication::translate("MainWindow", "Run Whole Slide", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("MainWindow", "Main view", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Recognition", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Smooth/defragment:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "iteration(s)", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Min target size:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "micron(s)", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Max target size:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QString());
        label_9->setText(QApplication::translate("MainWindow", "micron(s)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Thread count", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Visualization", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Image resolution:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "micron(s)", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("MainWindow", "Options view", 0, QApplication::UnicodeUTF8));
        dockWidget_3->setWindowTitle(QApplication::translate("MainWindow", "Result window", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Image Modality", 0, QApplication::UnicodeUTF8));
        type->clear();
        type->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Density distribution of stains", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Size distribution of stains", 0, QApplication::UnicodeUTF8)
        );
        open_reco_result->setText(QApplication::translate("MainWindow", "Open...", 0, QApplication::UnicodeUTF8));
        save_reco_result->setText(QApplication::translate("MainWindow", "Save...", 0, QApplication::UnicodeUTF8));
        contour->setText(QApplication::translate("MainWindow", "contour", 0, QApplication::UnicodeUTF8));
        flip->setText(QApplication::translate("MainWindow", "flip", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Orientation:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Scale bar min/max:", 0, QApplication::UnicodeUTF8));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_6), QApplication::translate("MainWindow", "Image", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_7), QApplication::translate("MainWindow", "Report", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

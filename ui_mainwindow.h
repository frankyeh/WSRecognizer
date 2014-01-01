/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Jan 1 09:59:56 2014
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
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *actionBatch_analysis;
    QAction *actionBatch_generate_images;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *show_recog;
    QSpacerItem *horizontalSpacer_2;
    QGraphicsView *main_view;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuRecent_Files;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *show_map_mask;
    QGraphicsView *graphicsView;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QGraphicsView *info_view;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    QTableWidget *info_widget;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget_2;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout;
    QToolButton *new_model;
    QToolButton *open_model;
    QToolButton *save_model;
    QPushButton *recognize_stains;
    QSpacerItem *horizontalSpacer;
    QGraphicsView *train_view;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_11;
    QSpinBox *smoothing;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QSpinBox *min_size;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_7;
    QSpinBox *max_size;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QProgressBar *progressBar;
    QPushButton *run;
    QLabel *label;
    QSpinBox *thread_count;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_5;
    QToolButton *open_reco_result;
    QPushButton *save_reco_result;
    QLabel *label_2;
    QDoubleSpinBox *color_min;
    QLabel *label_3;
    QDoubleSpinBox *color_max;
    QLabel *label_4;
    QSpinBox *resolution;
    QLabel *label_5;
    QToolButton *update_image;
    QSpacerItem *horizontalSpacer_3;
    QGraphicsView *result_view;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(752, 470);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        actionBatch_analysis = new QAction(MainWindow);
        actionBatch_analysis->setObjectName(QString::fromUtf8("actionBatch_analysis"));
        actionBatch_generate_images = new QAction(MainWindow);
        actionBatch_generate_images->setObjectName(QString::fromUtf8("actionBatch_generate_images"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(6, 6, 6, 6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
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

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_7->addLayout(horizontalLayout_2);

        main_view = new QGraphicsView(widget);
        main_view->setObjectName(QString::fromUtf8("main_view"));

        verticalLayout_7->addWidget(main_view);


        verticalLayout_2->addWidget(widget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 752, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuRecent_Files = new QMenu(menu_File);
        menuRecent_Files->setObjectName(QString::fromUtf8("menuRecent_Files"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(dockWidgetContents);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        show_map_mask = new QCheckBox(tab);
        show_map_mask->setObjectName(QString::fromUtf8("show_map_mask"));
        show_map_mask->setChecked(true);

        horizontalLayout_3->addWidget(show_map_mask);


        verticalLayout_4->addLayout(horizontalLayout_3);

        graphicsView = new QGraphicsView(tab);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout_4->addWidget(graphicsView);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        info_view = new QGraphicsView(tab_2);
        info_view->setObjectName(QString::fromUtf8("info_view"));

        verticalLayout_5->addWidget(info_view);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        info_widget = new QTableWidget(tab_3);
        if (info_widget->columnCount() < 2)
            info_widget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        info_widget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        info_widget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        info_widget->setObjectName(QString::fromUtf8("info_widget"));

        verticalLayout_6->addWidget(info_widget);

        tabWidget->addTab(tab_3, QString());

        verticalLayout->addWidget(tabWidget);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QString::fromUtf8("dockWidget_2"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget_2 = new QTabWidget(dockWidgetContents_2);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_8 = new QVBoxLayout(tab_4);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        new_model = new QToolButton(tab_4);
        new_model->setObjectName(QString::fromUtf8("new_model"));
        new_model->setMaximumSize(QSize(23, 22));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/new.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        new_model->setIcon(icon);

        horizontalLayout->addWidget(new_model);

        open_model = new QToolButton(tab_4);
        open_model->setObjectName(QString::fromUtf8("open_model"));
        open_model->setMaximumSize(QSize(23, 22));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/open.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        open_model->setIcon(icon1);

        horizontalLayout->addWidget(open_model);

        save_model = new QToolButton(tab_4);
        save_model->setObjectName(QString::fromUtf8("save_model"));
        save_model->setMaximumSize(QSize(23, 22));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/save.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        save_model->setIcon(icon2);

        horizontalLayout->addWidget(save_model);

        recognize_stains = new QPushButton(tab_4);
        recognize_stains->setObjectName(QString::fromUtf8("recognize_stains"));
        recognize_stains->setMaximumSize(QSize(16777215, 22));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/run.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        recognize_stains->setIcon(icon3);

        horizontalLayout->addWidget(recognize_stains);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_8->addLayout(horizontalLayout);

        train_view = new QGraphicsView(tab_4);
        train_view->setObjectName(QString::fromUtf8("train_view"));

        verticalLayout_8->addWidget(train_view);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_11 = new QLabel(tab_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_7->addWidget(label_11);

        smoothing = new QSpinBox(tab_4);
        smoothing->setObjectName(QString::fromUtf8("smoothing"));
        smoothing->setMaximum(10);
        smoothing->setSingleStep(1);
        smoothing->setValue(5);

        horizontalLayout_7->addWidget(smoothing);

        label_12 = new QLabel(tab_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_7->addWidget(label_12);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);


        verticalLayout_8->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(tab_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        min_size = new QSpinBox(tab_4);
        min_size->setObjectName(QString::fromUtf8("min_size"));
        min_size->setMaximum(50);
        min_size->setSingleStep(5);

        horizontalLayout_6->addWidget(min_size);

        label_8 = new QLabel(tab_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_6->addWidget(label_8);

        label_10 = new QLabel(tab_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_6->addWidget(label_10);

        label_7 = new QLabel(tab_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_6->addWidget(label_7);

        max_size = new QSpinBox(tab_4);
        max_size->setObjectName(QString::fromUtf8("max_size"));
        max_size->setMaximum(1000);
        max_size->setSingleStep(5);
        max_size->setValue(100);

        horizontalLayout_6->addWidget(max_size);

        label_9 = new QLabel(tab_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_6->addWidget(label_9);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_8->addLayout(horizontalLayout_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        progressBar = new QProgressBar(tab_4);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        horizontalLayout_4->addWidget(progressBar);

        run = new QPushButton(tab_4);
        run->setObjectName(QString::fromUtf8("run"));

        horizontalLayout_4->addWidget(run);

        label = new QLabel(tab_4);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        thread_count = new QSpinBox(tab_4);
        thread_count->setObjectName(QString::fromUtf8("thread_count"));
        thread_count->setMinimum(1);
        thread_count->setMaximum(10);

        horizontalLayout_4->addWidget(thread_count);


        verticalLayout_8->addLayout(horizontalLayout_4);

        tabWidget_2->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        verticalLayout_9 = new QVBoxLayout(tab_5);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        open_reco_result = new QToolButton(tab_5);
        open_reco_result->setObjectName(QString::fromUtf8("open_reco_result"));
        open_reco_result->setMaximumSize(QSize(23, 22));
        open_reco_result->setIcon(icon1);

        horizontalLayout_5->addWidget(open_reco_result);

        save_reco_result = new QPushButton(tab_5);
        save_reco_result->setObjectName(QString::fromUtf8("save_reco_result"));
        save_reco_result->setMaximumSize(QSize(23, 22));
        save_reco_result->setIcon(icon2);

        horizontalLayout_5->addWidget(save_reco_result);

        label_2 = new QLabel(tab_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_5->addWidget(label_2);

        color_min = new QDoubleSpinBox(tab_5);
        color_min->setObjectName(QString::fromUtf8("color_min"));

        horizontalLayout_5->addWidget(color_min);

        label_3 = new QLabel(tab_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_5->addWidget(label_3);

        color_max = new QDoubleSpinBox(tab_5);
        color_max->setObjectName(QString::fromUtf8("color_max"));
        color_max->setDecimals(1);
        color_max->setMaximum(50);
        color_max->setValue(5);

        horizontalLayout_5->addWidget(color_max);

        label_4 = new QLabel(tab_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_5->addWidget(label_4);

        resolution = new QSpinBox(tab_5);
        resolution->setObjectName(QString::fromUtf8("resolution"));
        resolution->setMinimum(10);
        resolution->setMaximum(100);
        resolution->setSingleStep(10);
        resolution->setValue(40);

        horizontalLayout_5->addWidget(resolution);

        label_5 = new QLabel(tab_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        update_image = new QToolButton(tab_5);
        update_image->setObjectName(QString::fromUtf8("update_image"));
        update_image->setMaximumSize(QSize(16777215, 22));

        horizontalLayout_5->addWidget(update_image);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_9->addLayout(horizontalLayout_5);

        result_view = new QGraphicsView(tab_5);
        result_view->setObjectName(QString::fromUtf8("result_view"));

        verticalLayout_9->addWidget(result_view);

        tabWidget_2->addTab(tab_5, QString());

        verticalLayout_3->addWidget(tabWidget_2);

        dockWidget_2->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_2);

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
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        action_Open->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
        actionBatch_analysis->setText(QApplication::translate("MainWindow", "Batch analysis...", 0, QApplication::UnicodeUTF8));
        actionBatch_generate_images->setText(QApplication::translate("MainWindow", "Batch generate images...", 0, QApplication::UnicodeUTF8));
        show_recog->setText(QApplication::translate("MainWindow", "Show recognition", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuRecent_Files->setTitle(QApplication::translate("MainWindow", "Recent Files", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "Slide Overview", 0, QApplication::UnicodeUTF8));
        show_map_mask->setText(QApplication::translate("MainWindow", "Show mask", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = info_widget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Property", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = info_widget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Page", 0, QApplication::UnicodeUTF8));
        dockWidget_2->setWindowTitle(QApplication::translate("MainWindow", "Color Recognition", 0, QApplication::UnicodeUTF8));
        new_model->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        open_model->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        save_model->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        recognize_stains->setText(QApplication::translate("MainWindow", "Recognize Stains", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Smoothing:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "iteration(s)", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Min Size:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "micron(s)", 0, QApplication::UnicodeUTF8));
        label_10->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "Max Size", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "microns(s)", 0, QApplication::UnicodeUTF8));
        run->setText(QApplication::translate("MainWindow", "Run", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Thread Count", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("MainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        open_reco_result->setText(QString());
        save_reco_result->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "min:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "max:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "resolution:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "mm", 0, QApplication::UnicodeUTF8));
        update_image->setText(QApplication::translate("MainWindow", "Update", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("MainWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

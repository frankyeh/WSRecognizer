#-------------------------------------------------
#
# Project created by QtCreator 2013-10-26T00:29:21
#
#-------------------------------------------------

QT       += core gui printsupport
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WSRecognizer
TEMPLATE = app

INCLUDEPATH += ../include \
                plot
SOURCES += main.cpp\
        mainwindow.cpp \
    wsi.cpp \
    qmapgraphicsscene.cpp \
    train_model.cpp \
    qtrainscene.cpp \
    qmainscene.cpp \
    rec.cpp \
    libs/prog_interface.cpp \
    gen.cpp \
    rec_dialog.cpp \
    gen_dialog.cpp \
    plot/qcustomplot.cpp \
    ../include/image/ml/svm.cpp \
    ana.cpp

HEADERS  += mainwindow.h \
    openslide-features.h \
    openslide.h \
    wsi.hpp \
    qmapgraphicsscene.h \
    train_model.hpp \
    qtrainscene.h \
    qmainscene.h \
    rec_dialog.hpp \
    gen_dialog.hpp \
    plot/qcustomplot.h \
    program_option.hpp \
    ../include/image/ml/cnn.hpp
RESOURCES += \
    icons.qrc
FORMS    += mainwindow.ui \
    rec_dialog.ui \
    gen_dialog.ui

OTHER_FILES +=

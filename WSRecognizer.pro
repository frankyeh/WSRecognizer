#-------------------------------------------------
#
# Project created by QtCreator 2013-10-26T00:29:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WSRecognizer
TEMPLATE = app

win32-g++ {
LIBS += -L. -lboost_thread-mgw45-mt-1_45.dll \
     -L. -lboost_program_options-mgw45-mt-1_45.dll \
}
INCLUDEPATH += C:/frank/myprog/include
SOURCES += main.cpp\
        mainwindow.cpp \
    wsi.cpp \
    qmapgraphicsscene.cpp \
    train_model.cpp \
    qtrainscene.cpp \
    qmainscene.cpp \
    rec.cpp

HEADERS  += mainwindow.h \
    openslide-features.h \
    openslide.h \
    wsi.hpp \
    qmapgraphicsscene.h \
    train_model.hpp \
    qtrainscene.h \
    qmainscene.h
RESOURCES += \
    icons.qrc
FORMS    += mainwindow.ui

OTHER_FILES +=

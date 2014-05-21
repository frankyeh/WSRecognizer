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
INCLUDEPATH += C:/frank/myprog/include\
                plot
SOURCES += main.cpp\
        mainwindow.cpp \
    wsi.cpp \
    qmapgraphicsscene.cpp \
    train_model.cpp \
    qtrainscene.cpp \
    qmainscene.cpp \
    rec.cpp \
    gzlib/zutil.c \
    gzlib/uncompr.c \
    gzlib/trees.c \
    gzlib/inftrees.c \
    gzlib/inflate.c \
    gzlib/inffast.c \
    gzlib/infback.c \
    gzlib/gzwrite.c \
    gzlib/gzread.c \
    gzlib/gzlib.c \
    gzlib/gzclose.c \
    gzlib/deflate.c \
    gzlib/crc32.c \
    gzlib/compress.c \
    gzlib/adler32.c \
    libs/prog_interface.cpp \
    gen.cpp \
    rec_dialog.cpp \
    ../../../include/image/ml/svm.cpp \
    gen_dialog.cpp \
    plot/qcustomplot.cpp

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
    plot/qcustomplot.h
RESOURCES += \
    icons.qrc
FORMS    += mainwindow.ui \
    rec_dialog.ui \
    gen_dialog.ui

OTHER_FILES +=

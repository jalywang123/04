QT += core
QT -= gui

TARGET = codebook
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    codebook.cpp \
    tags.cpp \
    track.cpp \
    tmain.cpp

HEADERS += \
    codebook.h \
    track.hpp


INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
               /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so \
        /usr/local/lib/libopencv_videoio.so





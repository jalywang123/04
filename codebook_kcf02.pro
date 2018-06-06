QT += core
QT -= gui

TARGET = codebook_kcf02
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tags.cpp \
    codebook.cpp \
    kcf/kcftracker.cpp \
    kcf/fhog.cpp \
    trackhan.cpp \
    socket.cpp

HEADERS += \
    track.hpp \
    codebook.h \
    kcf/tracker.h \
    kcf/recttools.hpp \
    kcf/labdata.hpp \
    kcf/kcftracker.hpp \
    kcf/fhog.hpp \
    kcf/ffttools.hpp \
    socket.h

INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
               /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so \
        /usr/local/lib/libopencv_videoio.so

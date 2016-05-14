#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T21:23:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    digitrecognition.cpp

HEADERS  += mainwindow.h \
    digitrecognition.h

FORMS    += mainwindow.ui
INCLUDEPATH += $$PWD/../opencv-3.1.0/build/install/include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../opencv-3.1.0/build/install/x86/vc12/lib/ -lopencv_aruco310d \
-lopencv_core310 \
-lopencv_imgcodecs310 \
-lopencv_highgui310 \
-lopencv_dnn310 -lopencv_imgproc310

else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../opencv-3.1.0/build/install/x86/vc12/lib/ -lopencv_aruco310d \
-lopencv_core310d \
-lopencv_imgcodecs310d \
-lopencv_highgui310d \
-lopencv_dnn310d -lopencv_imgproc310d
else:unix: LIBS += -L$$PWD/../opencv-3.1.0/build/install/x86/vc12/lib/ -lopencv_aruco310

DEPENDPATH += $$PWD/../opencv-3.1.0/build/install/x86/vc12

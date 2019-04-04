#-------------------------------------------------
#
# Project created by QtCreator 2019-03-19T12:17:25
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = projet_game
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG   += console
CONFIG   -= app_bundle

SOURCES += \
        main.cpp \
        game_mainwindow.cpp \
    game.cpp \
    Viewer.cpp

HEADERS += \
        game_mainwindow.h \
    game.hpp \
    Viewer.hpp

FORMS += \
        game_mainwindow.ui

INCLUDEPATH += /usr/local/include
INCLUDEPATH += .

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_objdetect

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

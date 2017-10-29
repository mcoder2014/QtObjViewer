#-------------------------------------------------
#
# Project created by QtCreator 2017-10-27T18:14:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = objViewer          # 生成的目标名称
TEMPLATE = app              # 编译类型 app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../bin
MOC_DIR = ./moc
OBJECTS_DIR = ./tmp
RCC_DIR = ./tmp

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    OpenGLWidget.cpp \
    Vertex.cpp

HEADERS += \
        mainwindow.h \
    OpenGLWidget.h \
    Vertex.h

FORMS += \
        mainwindow.ui

TRANSLATIONS += objViewer_cn.ts

RESOURCES += \
    objviewer.qrc

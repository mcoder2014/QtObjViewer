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

INCLUDEPATH += \
    ./ \
    E:/libs/assimp-3.3/include \      # assimp的Include文件夹的路径
    E:/libs/OpenMesh-6.3/include \    # openMesh的Include 的文件夹路径
    $$PWD/../libs/glm

LIBS += -LE:/libs/assimp-3.3/build/code/Release -lassimp-vc140-mt \  # assimp的lib文件
        -LE:\libs\OpenMesh-6.3\lib -lOpenMeshCore -lOpenMeshTools
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    OpenGLWidget.cpp \
    Vertex.cpp \
    Transform3D.cpp \
    Camera3D.cpp \
    Input.cpp \
    OpenGLMesh.cpp \
    objLoader.cpp \
    CustomMesh.cpp \
    CustomTexture.cpp

HEADERS += \
        mainwindow.h \
    OpenGLWidget.h \
    Vertex.h \
    Transform3D.h \
    Camera3D.h \
    Input.h \
    OpenGLMesh.h \
    objLoader.h \
    CustomMesh.h \
    CustomTexture.h

FORMS += \
        mainwindow.ui

TRANSLATIONS += objViewer_cn.ts

RESOURCES += \
    objviewer.qrc

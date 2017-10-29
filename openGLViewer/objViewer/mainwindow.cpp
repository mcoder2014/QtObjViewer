#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "OpenGLWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::init
///     初始化
///
void MainWindow::init()
{
    this->openglwidget = new OpenGLWidget();
    this->setCentralWidget(this->openglwidget);
}

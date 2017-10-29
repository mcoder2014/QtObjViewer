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

    this->setWindowTitle(tr("ObjViewer"));
    this->resize(1200,800);

    this->menu_file = new QMenu(tr("File"));

    this->action_open_file = new QAction(tr("OpenFile"));
    this->action_open_file->setStatusTip(tr("open 3d model file"));

    this->action_save = new QAction(tr("Save"));
    this->action_save->setStatusTip(tr("save file"));

    this->action_save_as = new QAction(tr("Save as"));
    this->action_save_as ->setStatusTip(tr("save model as file"));

    this->menu_file->addAction(this->action_open_file);
    this->menu_file->addAction(this->action_save);
    this->menu_file->addAction(this->action_save_as);
    ui->menuBar->addMenu(this->menu_file);

    ui->mainToolBar->addAction(this->action_open_file);
    ui->mainToolBar->addAction(this->action_save);
    ui->mainToolBar->addAction(this->action_save_as);

}

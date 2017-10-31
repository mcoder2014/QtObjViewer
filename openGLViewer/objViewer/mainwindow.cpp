#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "OpenGLWidget.h"
#include "objLoader.h"
#include <QFileDialog>

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

void MainWindow::openObjFile()
{
    objLoader loader;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);     // 打开文件模式
    fileDialog->setFileMode(QFileDialog::ExistingFile);     // 显示存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);           // 显示详细模式
    fileDialog->setNameFilter(tr("obj Files(*.obj)"));  // 过滤图片
    fileDialog->setWindowTitle(tr("Choose one obj file and open it"));    // 对话框标题

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];      // 用户选择的文件名

        qDebug() << "selected file:"
                 << path;
        loader.loadFile(path);
    }
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
    connect(this->action_open_file, SIGNAL(triggered(bool)),
            this, SLOT(openObjFile()));

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

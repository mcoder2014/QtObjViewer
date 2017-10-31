#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>

class OpenGLWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openObjFile();

private:
    Ui::MainWindow *ui;

    OpenGLWidget *openglwidget;     // openglWindow

    // menu
    QMenu * menu_file;       // 文件


    //action
    QAction *action_open_file;      // 打开文件
    QAction *action_save;           // 保存文件
    QAction *action_save_as;        // 另存为

    void init();

};

#endif // MAINWINDOW_H

#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class QOpenGLShaderProgram;

class OpenGLWidget
        : public QOpenGLWidget,
        public QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWidget();
    ~OpenGLWidget();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected slots:
    void teardownGL();

private:
    // OpenGL State Information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    void printContextInformation();     // 输出系统版本信息
};

#endif // OPENGLWIDGET_H

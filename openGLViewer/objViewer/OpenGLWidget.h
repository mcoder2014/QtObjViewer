#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "Transform3D.h"

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
    void update();

private:
    // OpenGL State Information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    // Shader Information
    int u_modelToWorld;
    int u_worldToView;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    void printContextInformation();     // 输出系统版本信息
};

#endif // OPENGLWIDGET_H

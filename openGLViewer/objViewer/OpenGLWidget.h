#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "Transform3D.h"
#include "Camera3D.h"

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

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    // OpenGL State Information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    // Shader Information
    int u_modelToWorld;
    int u_worldToCamera;
    int u_cameraToView;
//    int u_worldToView;
    QMatrix4x4 m_projection;
    Transform3D m_transform;
    Camera3D m_camera;

    void printContextInformation();     // 输出系统版本信息
};

#endif // OPENGLWIDGET_H

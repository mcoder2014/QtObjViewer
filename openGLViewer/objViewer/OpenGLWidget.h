#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "Transform3D.h"
#include "Camera3D.h"
#include "OpenGLMesh.h"
#include "CustomMesh.h"         // 自定义的mesh

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

public slots:
    void loadMesh();    // 创建一个 Mesh 对象测试

protected slots:
    void teardownGL();
    void update();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    // Cached Uniforms 三个变换的流程
    int u_modelToWorld;
    int u_worldToCamera;
    int u_cameraToView;
//    int u_worldToView;

    // Transformations
    QMatrix4x4 m_projection;
    Transform3D m_transform;
    Camera3D m_camera;

    // OpenGL 状态信息
//    QOpenGLBuffer m_vertex;
//    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;
//    OpenGLMesh *m_openMesh;             // 先只允许一个mesh
    CustomMesh *m_mesh;                 // 一个 mesh

    void printContextInformation();     // 输出系统版本信息
};

#endif // OPENGLWIDGET_H

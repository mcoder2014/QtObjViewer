#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class OpenglWindow
        : public QOpenGLWindow
{
    Q_OBJECT

public:
    OpenglWindow();
    ~OpenglWindow();

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void teardownGL();

private:
  // Private Helpers
  void printContextInformation();

  // OpenGL State Information
  QOpenGLBuffer m_vertex;
  QOpenGLVertexArrayObject m_object;
  QOpenGLShaderProgram *m_program;
};

#endif // OPENGLWINDOW_H

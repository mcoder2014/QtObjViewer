#ifndef OPENGLMESH_H
#define OPENGLMESH_H

#include <vector>
#include <QOpenGLBuffer>
#include <QObject>

class QOpenGLVertexArrayObject;
class OpenGLMeshPrivate;            // 封装了一些数据在这儿
class OpenGLMesh
        : public QObject
{
  Q_OBJECT
public:
  enum Options
  {
    Contiguous,
    Interleaved,
    Indexed
  };

  typedef std::vector<QOpenGLBuffer> OpenGLBufferList;

  explicit OpenGLMesh(QObject *parent = 0);
  OpenGLBufferList const &getBuffers() const;
  QOpenGLVertexArrayObject *vertexArrayObject();
  QOpenGLBuffer createBuffer(
          QOpenGLBuffer::UsagePattern hint = QOpenGLBuffer::StaticDraw);
  void setMode(OpenGLMesh::Options options);
  void setDrawArrays(GLenum mode, int count);
  void draw();
private:
  OpenGLMeshPrivate *m_private;
};

inline OpenGLMesh::Options operator|(
        OpenGLMesh::Options lhs, OpenGLMesh::Options rhs)
{
  return static_cast<OpenGLMesh::Options>(
              static_cast<int>(lhs) | static_cast<int>(rhs));
}

#endif // OPENGLMESH_H

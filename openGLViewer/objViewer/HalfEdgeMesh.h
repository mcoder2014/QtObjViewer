#ifndef KHALFEDGEMESH_H
#define KHALFEDGEMESH_H

#include "AbstractMesh.h"
#include <QVector3D>

class QString;

class HalfEdgeMeshPrivate;
class HalfEdgeMesh : public AbstractMesh
{
private:

  // Private Type Definitions
  struct IndexType
  {
    typedef AbstractMesh::index_type index_type;
    inline IndexType(index_type value);
    inline operator index_type const&() const;
    index_type m_value;
  };

public:

  // Public Type Definitions (Indices)
  struct VertexIndex : public IndexType
  {
    inline VertexIndex(index_type value);
  };
  struct EdgeIndex : public IndexType
  {
    inline EdgeIndex(index_type value);
  };
  struct HalfEdgeIndex : public IndexType
  {
    inline HalfEdgeIndex(index_type value);
  };
  struct FaceIndex : public IndexType
  {
    inline FaceIndex(index_type value);
  };

  // Public Type Definitions (Elements)
  struct Vertex
  {
    inline Vertex(QVector3D const &p, HalfEdgeIndex const& t);
    QVector3D position;
    HalfEdgeIndex to;
  };
  struct HalfEdge
  {
    inline HalfEdge(VertexIndex const &to);
    VertexIndex to;
    FaceIndex face;
    HalfEdgeIndex next;
  };
  struct Edge
  {
    inline Edge(VertexIndex const &a, VertexIndex const &b);
    HalfEdge a;
    HalfEdge b;
  };
  struct Face
  {
    inline Face(HalfEdgeIndex const &f);
    HalfEdgeIndex first;
  };

  // Public Type Definitions (Containers)
  typedef std::vector<Vertex> VertexContainer;
  typedef std::vector<HalfEdge> HalfEdgeContainer;
  typedef std::vector<Edge> EdgeContainer;
  typedef std::vector<Face> FaceContainer;

public:

  // Constructors / Destructor
  HalfEdgeMesh(QObject *parent, const QString &fileName);
  ~HalfEdgeMesh();

  // Add Commands (Does not check if value already exists!)
  VertexIndex addVertex(const QVector3D &v);
  FaceIndex addFace(index_array &a, index_array &b, index_array &c);

  // Query Commands (start from 1)
  Vertex const *vertex(VertexIndex idx) const;
  HalfEdge const *halfEdge(HalfEdgeIndex idx) const;
  const HalfEdgeIndex twin(HalfEdgeIndex idx) const;
  Edge const *edge(EdgeIndex idx) const;
  Face const *face(FaceIndex idx) const;
  VertexContainer const &vertices() const;
  HalfEdgeContainer const &halfEdges() const;
  EdgeContainer const &edges() const;
  FaceContainer const &faces() const;

  // Creates the OpenGL Mesh
  virtual OpenGLMesh* createOpenGLMesh(OpenGLMesh::Options options);

private:
  HalfEdgeMeshPrivate *m_private;
};

inline HalfEdgeMesh::IndexType::IndexType(index_type value) :
  m_value(value)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::IndexType::operator index_type const&() const
{
  return m_value;
}

inline HalfEdgeMesh::VertexIndex::VertexIndex(index_type value) :
  HalfEdgeMesh::IndexType(value)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::EdgeIndex::EdgeIndex(index_type value) :
  KHalfEdgeMesh::IndexType(value)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::HalfEdgeIndex::HalfEdgeIndex(index_type value) :
  HalfEdgeMesh::IndexType(value)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::FaceIndex::FaceIndex(index_type value) :
  HalfEdgeMesh::IndexType(value)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::Vertex::Vertex(KVector3D const &p, HalfEdgeIndex const& t) :
  position(p), to(t)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::HalfEdge::HalfEdge(VertexIndex const &to) :
  to(to), face(0), next(0)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::Edge::Edge(VertexIndex const &_a, VertexIndex const &_b) :
  a(_a), b(_b)
{
  // Intentionally Empty
}

inline HalfEdgeMesh::Face::Face(HalfEdgeIndex const &f) :
  first(f)
{
  // Intentionally Empty
}

#endif // KHALFEDGEMESH_H

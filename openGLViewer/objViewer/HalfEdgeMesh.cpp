#include "HalfEdgeMesh.h"
#include <QFile>

#include <unordered_map>
#include <algorithm>
#include "Vertex.h"
#include <vector>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "openglmesh.h"
#include <QOpenGLFunctions>

/*******************************************************************************
 * Indices (Fast Lookup)
 ******************************************************************************/
struct Indices
{
public:

    // Typedefs
    typedef HalfEdgeMesh::index_type index_type;
    typedef HalfEdgeMesh::index_pair index_pair;

    // Member Information
    union
    {
        struct
        {
            index_type high;
            index_type low;
        };
        index_pair all;
    };

    // Constructor / Operators
    inline Indices(index_type from, index_type to);
    inline bool operator<(const Indices &rhs) const;
    inline bool operator==(const Indices &rhs) const;
};

inline Indices::Indices(index_type from, index_type to)
{
    if (from > to)
    {
        high = from;
        low = to;
    }
    else
    {
        high = to;
        low = from;
    }
}

inline bool Indices::operator<(const Indices &rhs) const
{
    return all < rhs.all;
}
inline bool Indices::operator==(const Indices &rhs) const
{
    return all == rhs.all;
}

struct IndicesHash : public std::unary_function<Indices::index_pair const&, Indices const&>
{
    inline Indices::index_pair const& operator()(Indices const& rhs) const
    {
        return rhs.all;
    }
};

/*******************************************************************************
 * HalfEdgeMeshPrivate
 ******************************************************************************/
class HalfEdgeMeshPrivate
{
public:

    // typedefs (Fundamental)
    typedef HalfEdgeMesh::index_type index_type;
    typedef HalfEdgeMesh::index_pair index_pair;
    typedef HalfEdgeMesh::index_array index_array;
    // typedefs (Indices)
    typedef HalfEdgeMesh::VertexIndex VertexIndex;
    typedef HalfEdgeMesh::EdgeIndex EdgeIndex;
    typedef HalfEdgeMesh::HalfEdgeIndex HalfEdgeIndex;
    typedef HalfEdgeMesh::FaceIndex FaceIndex;
    // typedefs (Elements)
    typedef HalfEdgeMesh::Vertex Vertex;
    typedef HalfEdgeMesh::HalfEdge HalfEdge;
    typedef HalfEdgeMesh::Edge Edge;
    typedef HalfEdgeMesh::Face Face;
    // typedefs (Containers)
    typedef HalfEdgeMesh::VertexContainer VertexContainer;
    typedef HalfEdgeMesh::HalfEdgeContainer HalfEdgeContainer;
    typedef HalfEdgeMesh::EdgeContainer EdgeContainer;
    typedef HalfEdgeMesh::FaceContainer FaceContainer;
    typedef std::unordered_map<Indices,EdgeIndex,IndicesHash> EdgeLookup;

    // Add Commands (Does not check if value already exists!)
    VertexIndex addVertex(const QVector3D &v);
    EdgeIndex addEdge(const index_array &from, const index_array &to);
    HalfEdgeIndex addHalfEdge(const index_array &from, const index_array &to);
    FaceIndex addFace(index_array &a, index_array &b, index_array &c);

    // Query Commands (start from 1)
    Vertex *vertex(VertexIndex const &idx);
    Edge *edge(EdgeIndex const &idx);
    HalfEdge *twin(HalfEdge *idx);
    HalfEdge *halfEdge(HalfEdgeIndex const &idx);
    Face *face(FaceIndex const &idx);
    Vertex const *vertex(VertexIndex const &idx) const;
    Edge const *edge(EdgeIndex const &idx) const;
    HalfEdgeIndex twin(HalfEdgeIndex const &idx) const;
    HalfEdge const *twin(HalfEdge const *idx) const;
    HalfEdge const *halfEdge(HalfEdgeIndex const &idx) const;
    Face const *face(FaceIndex const &idx) const;
    VertexIndex index(Vertex const *v) const;
    EdgeIndex index(Edge const *e) const;
    HalfEdgeIndex index(HalfEdge const *he) const;
    FaceIndex index(Face const *f) const;
    VertexContainer const &vertices() const;
    EdgeContainer const &edges() const;
    FaceContainer const &faces() const;

    // Get Commands (start from 1, queries first)
    HalfEdgeIndex findHalfEdge(const index_array &from, const index_array &to);
    HalfEdgeIndex getHalfEdge(const index_array &from, const index_array &to);

    // Helpers
    void normalizeIndex(KAbstractMesh::index_type &v, size_t const &sizePlusOne);
    QVector3D calculateFaceNormal(const Face *face);
    QVector3D calculateVertexNormal(const Vertex *vertex);

private:
    VertexContainer m_vertices;
    EdgeContainer m_edges;
    FaceContainer m_faces;
    EdgeLookup m_edgeLookup;
};

HalfEdgeMeshPrivate::VertexIndex HalfEdgeMeshPrivate::addVertex(const QVector3D &v)
{
    m_vertices.emplace_back(v, 0);
    return m_vertices.size();
}

HalfEdgeMeshPrivate::EdgeIndex HalfEdgeMeshPrivate::addEdge(const index_array &from, const index_array &to)
{
    Indices idx(from[0], to[0]);
    m_edges.emplace_back(idx.low, idx.high);

    // Note: We want idx starting from 1.
    //       So we only sutract 1 from the total edges.
    EdgeIndex offset = m_edges.size();
    m_edgeLookup.emplace(idx, offset);

    return offset;
}

HalfEdgeMeshPrivate::HalfEdgeIndex HalfEdgeMeshPrivate::addHalfEdge(const index_array &from, const index_array &to)
{
    EdgeIndex edgeIndex = addEdge(from, to);

    // Convert to HalfEdgeIndex
    if (from[0] > to[0])
        return 2 * edgeIndex - 1;
    else
        return 2 * edgeIndex;
}

inline void HalfEdgeMeshPrivate::normalizeIndex(KAbstractMesh::index_type &v, size_t const &sizePlusOne)
{
    if (v < sizePlusOne) return;
    v %= sizePlusOne;
    ++v;
}

HalfEdgeMeshPrivate::FaceIndex HalfEdgeMeshPrivate::addFace(index_array &v1, index_array &v2, index_array &v3)
{
    // Normalize Indices
    size_t size = m_vertices.size() + 1;

    // Normalize Indices
    normalizeIndex(v1[0], size);
    normalizeIndex(v2[0], size);
    normalizeIndex(v3[0], size);

    // Create edges
    HalfEdgeIndex edgeA = getHalfEdge(v1, v2);
    HalfEdgeIndex edgeB = getHalfEdge(v2, v3);
    HalfEdgeIndex edgeC = getHalfEdge(v3, v1);

    // Set Vertex half edges
    if (vertex(v1[0])->to == 0) vertex(v1[0])->to = edgeA;
    if (vertex(v2[0])->to == 0) vertex(v2[0])->to = edgeB;
    if (vertex(v3[0])->to == 0) vertex(v3[0])->to = edgeC;

    // Create Face
    m_faces.emplace_back(edgeA);
    FaceIndex faceIdx = m_faces.size();

    // Initialize half edges
    HalfEdge *edge;
    edge = halfEdge(edgeA);
    edge->face = faceIdx;
    edge->next = edgeB;
    edge = halfEdge(edgeB);
    edge->face = faceIdx;
    edge->next = edgeC;
    edge = halfEdge(edgeC);
    edge->face = faceIdx;
    edge->next = edgeA;

    return faceIdx;
}

HalfEdgeMeshPrivate::Vertex *HalfEdgeMeshPrivate::vertex(const VertexIndex &idx)
{
    return &m_vertices[idx - 1];
}

HalfEdgeMeshPrivate::Edge *HalfEdgeMeshPrivate::edge(const EdgeIndex &idx)
{
    return &m_edges[idx - 1];
}

HalfEdgeMeshPrivate::HalfEdge *HalfEdgeMeshPrivate::twin(HalfEdge *edge)
{
    return halfEdge(twin(index(edge)));
}

HalfEdgeMeshPrivate::HalfEdge *HalfEdgeMeshPrivate::halfEdge(const HalfEdgeIndex &idx)
{
    return &reinterpret_cast<HalfEdge*>(m_edges.data())[idx - 1];
}

HalfEdgeMeshPrivate::Face *HalfEdgeMeshPrivate::face(const FaceIndex &idx)
{
    return &m_faces[idx - 1];
}

HalfEdgeMeshPrivate::Vertex const *HalfEdgeMeshPrivate::vertex(const VertexIndex &idx) const
{
    return &m_vertices[idx - 1];
}

HalfEdgeMeshPrivate::Edge const *HalfEdgeMeshPrivate::edge(const EdgeIndex &idx) const
{
    return &m_edges[idx - 1];
}

HalfEdgeMeshPrivate::HalfEdgeIndex HalfEdgeMeshPrivate::twin(const HalfEdgeIndex &idx) const
{
    if (idx % 2)
        return idx + 1;
    else
        return idx - 1;
}

HalfEdgeMeshPrivate::HalfEdge const *HalfEdgeMeshPrivate::twin(HalfEdge const *edge) const
{
    return halfEdge(twin(index(edge)));
}

HalfEdgeMeshPrivate::HalfEdge const *HalfEdgeMeshPrivate::halfEdge(const HalfEdgeIndex &idx) const
{
    return &reinterpret_cast<HalfEdge const*>(m_edges.data())[idx - 1];
}

HalfEdgeMeshPrivate::Face const *HalfEdgeMeshPrivate::face(const FaceIndex &idx) const
{
    return &m_faces[idx - 1];
}

HalfEdgeMeshPrivate::VertexIndex HalfEdgeMeshPrivate::index(Vertex const *v) const
{
    return (v - m_vertices.data()) + 1;
}

HalfEdgeMeshPrivate::EdgeIndex HalfEdgeMeshPrivate::index(Edge const *e) const
{
    return (e - m_edges.data()) + 1;
}

HalfEdgeMeshPrivate::HalfEdgeIndex HalfEdgeMeshPrivate::index(HalfEdge const *he) const
{
    return (he - reinterpret_cast<HalfEdge const*>(m_edges.data())) + 1;
}

HalfEdgeMeshPrivate::FaceIndex HalfEdgeMeshPrivate::index(Face const *f) const
{
    return (f - m_faces.data()) + 1;
}

HalfEdgeMeshPrivate::VertexContainer const &HalfEdgeMeshPrivate::vertices() const
{
    return m_vertices;
}

HalfEdgeMeshPrivate::EdgeContainer const &HalfEdgeMeshPrivate::edges() const
{
    return m_edges;
}

HalfEdgeMeshPrivate::FaceContainer const &HalfEdgeMeshPrivate::faces() const
{
    return m_faces;
}

HalfEdgeMeshPrivate::HalfEdgeIndex HalfEdgeMeshPrivate::findHalfEdge(const index_array &from, const index_array &to)
{
    EdgeLookup::const_iterator it = m_edgeLookup.find(Indices(from[0], to[0]));
    if (it == m_edgeLookup.end()) return 0;

    if (from[0] > to[0])
        return 2 * it->second - 1;
    else
        return 2 * it->second;
}

HalfEdgeMeshPrivate::HalfEdgeIndex HalfEdgeMeshPrivate::getHalfEdge(const index_array &from, const index_array &to)
{
    HalfEdgeIndex idx = findHalfEdge(from, to);
    if (idx == 0)
    {
        return addHalfEdge(from, to);
    }
    return idx;
}

QVector3D HalfEdgeMeshPrivate::calculateFaceNormal(const Face *face)
{
    const HalfEdge *edge = halfEdge(face->first);

    QVector3D pos1 = vertex(edge->to)->position;
    edge = halfEdge(edge->next);
    QVector3D pos2 = vertex(edge->to)->position;
    edge = halfEdge(edge->next);
    QVector3D pos3 = vertex(edge->to)->position;

    QVector3D a = pos2 - pos1;
    QVector3D b = pos3 - pos1;
    QVector3D c = QVector3D::crossProduct(a,b);
    c.normalize();

    return c;
}

QVector3D HalfEdgeMeshPrivate::calculateVertexNormal(const Vertex *vertex)
{
    HalfEdgeIndex currIndex(0), twinIndex(0);
    QVector3D normal;
    const HalfEdge *startEdge = halfEdge(vertex->to);
    const HalfEdge *edge = startEdge;
    do
    {
        if (edge->face != 0)
            normal += calculateFaceNormal(face(edge->face));
        currIndex = index(edge);
        twinIndex = twin(currIndex);
        edge = halfEdge(twinIndex);
        edge = halfEdge(edge->next);
    }
    while (edge != startEdge);
    normal.normalize();

    return normal;
}

/*******************************************************************************
 * Half Edge Mesh Public
 ******************************************************************************/
#define P(c) c &p = *m_private

HalfEdgeMesh::HalfEdgeMesh(QObject *parent, const QString &fileName) :
    KAbstractMesh(parent), m_private(new HalfEdgeMeshPrivate)
{
    KFileReader reader(fileName);
    KHalfEdgeObjParser parser(this, &reader);
    parser.parse();
}

HalfEdgeMesh::~HalfEdgeMesh()
{
    delete m_private;
}

HalfEdgeMesh::VertexIndex HalfEdgeMesh::addVertex(const QVector3D &v)
{
    P(HalfEdgeMeshPrivate);
    return p.addVertex(v);
}

HalfEdgeMesh::FaceIndex HalfEdgeMesh::addFace(index_array &a, index_array &b, index_array &c)
{
    P(HalfEdgeMeshPrivate);
    return p.addFace(a, b, c);
}

// Query Commands (start from 1)
HalfEdgeMesh::Vertex const *HalfEdgeMesh::vertex(VertexIndex idx) const
{
    P(HalfEdgeMeshPrivate);
    return p.vertex(idx);
}

const HalfEdgeMesh::HalfEdge *HalfEdgeMesh::halfEdge(HalfEdgeMesh::HalfEdgeIndex idx) const
{
    P(HalfEdgeMeshPrivate);
    return p.halfEdge(idx);
}

const HalfEdgeMesh::HalfEdgeIndex HalfEdgeMesh::twin(HalfEdgeMesh::HalfEdgeIndex idx) const
{
    P(HalfEdgeMeshPrivate);
    return p.twin(idx);
}

const HalfEdgeMesh::Edge *HalfEdgeMesh::edge(HalfEdgeMesh::EdgeIndex idx) const
{
    P(HalfEdgeMeshPrivate);
    return p.edge(idx);
}

HalfEdgeMesh::Face const *HalfEdgeMesh::face(FaceIndex idx) const
{
    P(HalfEdgeMeshPrivate);
    return p.face(idx);
}

HalfEdgeMesh::VertexContainer const &HalfEdgeMesh::vertices() const
{
    P(HalfEdgeMeshPrivate);
    return p.vertices();
}

const HalfEdgeMesh::HalfEdgeContainer &HalfEdgeMesh::halfEdges() const
{
    P(HalfEdgeMeshPrivate);
    return reinterpret_cast<HalfEdgeContainer const&>(p.edges());
}

HalfEdgeMesh::EdgeContainer const &HalfEdgeMesh::edges() const
{
    P(HalfEdgeMeshPrivate);
    return p.edges();
}

HalfEdgeMesh::FaceContainer const &HalfEdgeMesh::faces() const
{
    P(HalfEdgeMeshPrivate);
    return p.faces();
}

// Creates the OpenGL Mesh
OpenGLMesh* HalfEdgeMesh::createOpenGLMesh(OpenGLMesh::Options options)
{
    P(HalfEdgeMeshPrivate);
    FaceContainer const &faces = p.faces();
    size_t faceCount = faces.size();

    // Currently we don't support options
    (void)options;

    // Create information
    OpenGLFunctions f(QOpenGLContext::currentContext());
    OpenGLMesh *mesh = new OpenGLMesh(parent());
    OpenGLVertexArrayObject *vao = mesh->vertexArrayObject();
    OpenGLBuffer buffer = mesh->createBuffer();
    const HalfEdge *edge = Q_NULLPTR;

    // Load data into OpenGLBuffer
    vao->bind();
    {
        buffer.bind();

        // Send data to GPU
        QVector3D n;
        buffer.allocate(sizeof(KVertex) * faceCount * 3);
        KVertex *data = static_cast<KVertex*>(buffer.map(QOpenGLBuffer::WriteOnly));
        for (size_t i = 0; i < faceCount; ++i)
        {
            edge = p.halfEdge(faces[i].first);
            n = p.calculateFaceNormal(&faces[i]);
            data[3*i] = KVertex(vertex(edge->to)->position, n);
            edge = p.halfEdge(edge->next);
            data[3*i+1] = KVertex(vertex(edge->to)->position, n);
            edge = p.halfEdge(edge->next);
            data[3*i+2] = KVertex(vertex(edge->to)->position, n);
        }
        buffer.unmap();

        // Bind attributes
        f.glVertexAttribPointer(0, KVertex::PositionTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::positionOffset());
        f.glVertexAttribPointer(1, KVertex::ColorTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::colorOffset());
        f.glEnableVertexAttribArray(0);
        f.glEnableVertexAttribArray(1);
    }
    vao->release();

    // Initialize mesh
    mesh->setMode(OpenGLMesh::Contiguous | OpenGLMesh::Interleaved);
    mesh->setDrawArrays(GL_TRIANGLES, faceCount * 3);
    return mesh;
}

#ifndef ABSTRACTMESH_H
#define ABSTRACTMESH_H

#include <QObject>
#include "OpenGLMesh.h"


class AbstractMesh : public QObject
{
    Q_OBJECT
public:

    typedef uint32_t index_type;
    typedef uint64_t index_pair;
    typedef uint64_t size_type;
    typedef std::array<index_type, 3> index_array;

    typedef index_type index_type;
    typedef index_pair index_pair;
    typedef index_array index_array;

    explicit AbstractMesh(QObject *parent = 0);
    virtual OpenGLMesh* createOpenGLMesh(OpenGLMesh::Options options) = 0;
signals:

public slots:
};

inline AbstractMesh::KAbstractMesh(QObject *parent) :
    QObject(parent)
{
    // Intentionally Empty
}

#endif // ABSTRACTMESH_H

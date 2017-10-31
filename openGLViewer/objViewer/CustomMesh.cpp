#include "CustomMesh.h"

CustomMesh::CustomMesh(QObject *parent)
    :QObject(parent)
{
    this->inited = false;
    this->m_vao = NULL;
}

void CustomMesh::draw()
{
    if(this->inited == false)
        return;

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    this->m_vao->bind();
    f->glDrawArrays(this->m_mode, 0, this->m_count);
    this->m_vao->release();
}

void CustomMesh::setDrawArrays(GLenum mode, int count)
{
    this->m_mode = mode;
    this->m_count = count;
}

QOpenGLVertexArrayObject *CustomMesh::vertexArrayObject()
{
    if(this->m_vao == NULL)
        this->m_vao = new QOpenGLVertexArrayObject();

    return this->m_vao;
}

QOpenGLBuffer *CustomMesh::createBuffer(QOpenGLBuffer::UsagePattern hint)
{
    QOpenGLBuffer buffer;
    buffer.create();
    buffer.setUsagePattern(hint);
    this->m_buffer.push_back(buffer);

    return buffer;

}

std::vector<QOpenGLBuffer> *CustomMesh::buffers()
{
    return this->m_buffer;
}

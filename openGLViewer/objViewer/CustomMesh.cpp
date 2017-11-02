#include "CustomMesh.h"

CustomMesh::CustomMesh(QObject *parent)
    :QObject(parent)
{
    this->inited = false;
    this->m_vao = NULL;
    this->m_transform = new Transform3D();
}

void CustomMesh::draw(QOpenGLShaderProgram* program)
{
    if(this->inited == false)
        return;

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    this->m_vao->bind();
    if(this->m_textures.size() > 0)
    {
        CustomTexture* tex = this->m_textures[0];
        tex->texture->bind(0);
        program->setUniformValue("texture_diffuse",0);
    }

    f->glDrawArrays(this->m_mode, 0, this->m_count);
    this->m_vao->release();
    if(this->m_textures.size() > 0)
    {
        this->m_textures[0]->texture->release();
    }
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

    return &this->m_buffer[this->m_buffer.size() - 1];

}

std::vector<QOpenGLBuffer> *CustomMesh::buffers()
{
    return &this->m_buffer;
}

///
/// \brief CustomMesh::createCube
/// \param position
/// \param size
///
void CustomMesh::createCube(QVector3D position, float size)
{
    float halfsize = size / 2;      // 半边的长度
    float x = position.x();
    float y = position.y();
    float z = position.z();

    // front
    QVector3D FTR(halfsize + x, halfsize + y, halfsize + z);
    QVector3D FTL(-halfsize + x, halfsize + y, halfsize + z);
    QVector3D FBL(-halfsize + x, -halfsize + y, halfsize + z);
    QVector3D FBR(halfsize + x, -halfsize + y, halfsize + z);

    // back
    QVector3D BTR(halfsize + x, halfsize + y, -halfsize + z);
    QVector3D BTL(-halfsize + x, halfsize + y, -halfsize + z);
    QVector3D BBL(-halfsize + x, -halfsize + y, -halfsize + z);
    QVector3D BBR(halfsize + x, -halfsize + y, -halfsize + z);

    // color

    // Face 1 Front
    this->m_vertexs.push_back(Vertex(FTR));
    this->m_vertexs.push_back(Vertex(FTL));
    this->m_vertexs.push_back(Vertex(FBL));

    this->m_vertexs.push_back(Vertex(FBL));
    this->m_vertexs.push_back(Vertex(FBR));
    this->m_vertexs.push_back(Vertex(FTR));

    // face 2 back
    this->m_vertexs.push_back(Vertex(BBR));
    this->m_vertexs.push_back(Vertex(BTL));
    this->m_vertexs.push_back(Vertex(BTR));

    this->m_vertexs.push_back(Vertex(BTL));
    this->m_vertexs.push_back(Vertex(BBR));
    this->m_vertexs.push_back(Vertex(BBL));

    // face 3 top
    this->m_vertexs.push_back(Vertex(FTR));
    this->m_vertexs.push_back(Vertex(BTR));
    this->m_vertexs.push_back(Vertex(BTL));

    this->m_vertexs.push_back(Vertex(BTL));
    this->m_vertexs.push_back(Vertex(FTL));
    this->m_vertexs.push_back(Vertex(FTR));

    // face 4 bottom
    this->m_vertexs.push_back(Vertex(FBR));
    this->m_vertexs.push_back(Vertex(FBL));
    this->m_vertexs.push_back(Vertex(BBL));

    this->m_vertexs.push_back(Vertex(BBL));
    this->m_vertexs.push_back(Vertex(BBR));
    this->m_vertexs.push_back(Vertex(FBR));

    // face 5 left
    this->m_vertexs.push_back(Vertex(FBL));
    this->m_vertexs.push_back(Vertex(FTL));
    this->m_vertexs.push_back(Vertex(BTL));

    this->m_vertexs.push_back(Vertex(FBL));
    this->m_vertexs.push_back(Vertex(BTL));
    this->m_vertexs.push_back(Vertex(BBL));

    // face 6 right
    this->m_vertexs.push_back(Vertex(FTR));
    this->m_vertexs.push_back(Vertex(FBR));
    this->m_vertexs.push_back(Vertex(BBR));

    this->m_vertexs.push_back(Vertex(BBR));
    this->m_vertexs.push_back(Vertex(BTR));
    this->m_vertexs.push_back(Vertex(FTR));

    qDebug() <<"create vertex finished";

    this->setDrawArrays(GL_TRIANGLES, this->m_vertexs.size());
    QOpenGLFunctions *function = QOpenGLContext::currentContext()->functions();
    this->m_vao = new QOpenGLVertexArrayObject();
    this->m_vao->create();
    this->m_vao->bind();

    QOpenGLBuffer * buffer = this->createBuffer();
    buffer->create();
    buffer->bind();
    buffer->allocate(this->m_vertexs.size() * sizeof(Vertex));
    Vertex *data = static_cast<Vertex*>(buffer->map(QOpenGLBuffer::WriteOnly));
    for(int i = 0; i < m_vertexs.size(); i++)
    {
        Vertex vertex;
        QVector3D vec = m_vertexs[i].position();
        vertex.setPosition(vec);
        data[i] = vertex;
    }

    function->glVertexAttribPointer(
                0,
                Vertex::PositionTupleSize,
                GL_FLOAT,
                GL_FALSE,
                Vertex::stride(),
                (void*)Vertex::positionOffset());

    function->glVertexAttribPointer(
                1,
                Vertex::ColorTupleSize,
                GL_FLOAT,
                GL_FALSE,
                Vertex::stride(),
                (void*)Vertex::colorOffset());

    function->glVertexAttribPointer(
                2,
                Vertex::TexCoordsTupleSize,
                GL_FLOAT,
                GL_FALSE,
                Vertex::stride(),
                (void*)Vertex::texCoordsOffset());

    function->glEnableVertexAttribArray(0);
    function->glEnableVertexAttribArray(1);
    function->glEnableVertexAttribArray(2);

    buffer->unmap();

    this->inited = true;

    m_vao->release();
    buffer->release();
    qDebug() << "create cube finished";
}

Transform3D *CustomMesh::transform()
{
    return this->m_transform;
}

// 设置transform
void CustomMesh::setTransform(Transform3D &transform)
{
    this->m_transform->setTranslation(transform.translation());
    this->m_transform->setRotation(transform.rotation());
    this->m_transform->setScale(transform.scale());
}

std::vector<CustomTexture *> *CustomMesh::textures()
{
    return &this->m_textures;
}

CustomTexture *CustomMesh::texture(int index)
{
    return this->m_textures[index];
}

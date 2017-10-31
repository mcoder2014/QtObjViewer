#ifndef CUSTOMMESH_H
#define CUSTOMMESH_H

#include "Vertex.h"
#include "Transform3D.h"
#include "CustomTexture.h"
#include "objLoader.h"

#include <vector>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>


class CustomMesh
        :public QObject
{
    Q_OBJECT
public:
    CustomMesh(QObject *parent = 0);
    bool isInited(){return this->inited;}
    void draw();
    void setDrawArrays(GLenum mode, int count); // 设置渲染队列
    QOpenGLVertexArrayObject *vertexArrayObject();
    QOpenGLBuffer *createBuffer(
            QOpenGLBuffer::UsagePattern hint = QOpenGLBuffer::StaticDraw);
    std::vector<QOpenGLBuffer> *buffers();

private:
    std::vector<Vertex> m_vertexs;              // 点信息
    std::vector<CustomTexture> m_textures;      // 贴图信息
    std::vector<GLuint> indices;                // 索引信息

    std::vector<QOpenGLBuffer> m_buffer;        // 缓冲队列
    QOpenGLVertexArrayObject *m_vao;            // VAO
    bool inited;                                // 是否初始化
    GLenum m_mode;                              // 渲染模式?
    int m_count;                                // 点的数量

    friend class objLoader;

};

#endif // CUSTOMMESH_H

#include "objLoader.h"
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "CustomMesh.h"

objLoader::objLoader(QObject *parent) : QObject(parent)
{
    this->scene = NULL;
}

///
/// \brief objLoader::loadFile
/// \param filePath
/// \return
/// 读取文件
///
const aiScene *objLoader::loadFile(QString filePath)
{
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(
                filePath.toStdString(),
                aiProcess_CalcTangentSpace |
                aiProcess_Triangulate   |
                aiProcess_FlipUVs
                );

    if(!scene)
    {
        qDebug() << importer.GetErrorString();
        this->scene = NULL;
        return NULL;
    }
    else
    {
        this->scene = scene;
        qDebug() << "Scene details"
                 << "has animations: " << scene->HasAnimations() << scene->mNumAnimations
                 << "has camerass: " << scene->HasCameras() << scene->mNumCameras
                 << "has lights: " << scene->HasLights()    << scene->mNumLights
                 << "has meshes: " << scene->HasMeshes()    << scene->mNumMeshes
                 << "has materials: " << scene->HasMaterials() << scene->mNumMaterials
                 << "has textures: " << scene->HasTextures()    << scene->mNumTextures;
        this->readAiScene(scene);
        return this->scene;
    }
}

void objLoader::readAiScene(const aiScene *scene)
{
    if(scene == NULL)
    {
        return;
    }
    aiMesh ** meshes = scene->mMeshes;      // meshes数组

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        qDebug()<< "read mesh: "<< i;
        aiMesh* aimesh = meshes[i];
        qDebug() << "faces: " << aimesh->HasFaces() << aimesh->mNumFaces
                 << "position: " << aimesh->HasPositions()
                 << "vertices: " << aimesh->mNumVertices
                 << "textureCoords: " << aimesh->HasTextureCoords(0) << aimesh->HasTextureCoords(1)
                 << "hasVertexColors: " << aimesh->HasVertexColors(0) << aimesh->HasVertexColors(1)
                 << "material index: " << aimesh->mMaterialIndex;

//        int vertices_length = aimesh->mNumVertices;
//        aiVector3D *vectors = aimesh->mVertices;
//        aiVector3D *uvCoords = aimesh->mTextureCoords[0];
//        for(int j =0; j< vertices_length; j++)
//        {
//            qDebug() << vectors[j].x << " "<< vectors[j].y<< " " << vectors[j].z<< " "
//                     << uvCoords[j].x << " " <<uvCoords[j].y << " " << uvCoords[j].z ;
//        }

    }

    aiMaterial ** mats = scene->mMaterials; // materials

    for(int i = 0; i < scene->mNumMaterials; i++)
    {
        qDebug() << "read materials: "<< i;
        aiMaterial * mat = mats[i];
        qDebug() << "numproperties: " << mat->mNumProperties;
    }

    qDebug() <<"sizeof vectex" << sizeof(Vertex);

}

CustomMesh *objLoader::getMesh(const aiScene *scene)
{
    if(scene == NULL)
    {
        return NULL;
    }

    aiMesh ** meshes = scene->mMeshes;      // meshes数组

    CustomMesh *cmesh = new CustomMesh();   // 新建自定义网格
    QOpenGLFunctions *function = QOpenGLContext::currentContext()->functions();
    cmesh->m_vao = new QOpenGLVertexArrayObject();
    cmesh->m_vao->bind();   // 绑定 vao

    if(scene->mNumMeshes > 0)
    {
        // 当前只针对我们自己的情况
        aiMesh * aimesh = meshes[0];    // 我们的人脸模型放在第0个位置

        qDebug() << "faces: " << aimesh->HasFaces() << aimesh->mNumFaces
                 << "vertices: " << aimesh->mNumVertices
                 << "textureCoords: " << aimesh->HasTextureCoords(0) << aimesh->HasTextureCoords(1)
                 << "hasVertexColors: " << aimesh->HasVertexColors(0) << aimesh->HasVertexColors(1)
                 << "material index: " << aimesh->mMaterialIndex;

        // 开始处理点
        int vertices_length = aimesh->mNumVertices;
        aiVector3D *vectices = aimesh->mVertices;
        aiVector3D *uvCoords = aimesh->mTextureCoords[0];
        aiVector3D *normals = aimesh->mNormals;

        for( int vec = 0; vec < vertices_length; vec ++)
        {
            Vertex temp_v(
                        QVector3D(vectices[vec].x, vectices[vec].y, vectices[vec].z),
                        QVector3D(normals[vec].x, normals[vec].y, normals[vec].z),
                        QVector2D(uvCoords[vec].x, uvCoords[vec].y));

            cmesh->m_vertexs.emplace_back(temp_v);  // 加入
        }

        // 将数据拷贝到缓冲区
        QOpenGLBuffer *buffer = cmesh->createBuffer();
        buffer->bind();
        buffer->allocate( vertices_length * sizeof(Vertex));    // 先分配空间
        Vertex *data = static_cast<Vertex*>(buffer->map(QOpenGLBuffer::ReadWrite));
        for(size_t i = 0; i < vertices_length; i++)
        {
            data[i] = Vertex( QVector3D(vectices[i].x, vectices[i].y, vectices[i].z),
                              QVector3D(normals[i].x, normals[i].y, normals[i].z),
                              QVector2D(uvCoords[i].x, uvCoords[i].y));
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


    }

    aiMaterial ** mats = scene->mMaterials; // materials

    for(int i = 0; i < scene->mNumMaterials; i++)
    {
        qDebug() << "read materials: "<< i;
        aiMaterial * mat = mats[i];
        qDebug() << "numproperties: " << mat->mNumProperties;
    }

    cmesh->m_vao->release();    // vao 释放

    return NULL;
}

//CustomMesh *objLoader::getMesh(const aiScene *scene)
//{
//    if(scene == NULL)
//    {
//        return NULL;
//    }
//    aiMesh ** meshes = scene->mMeshes;      // meshes数组

//    CustomMesh *cmesh = new CustomMesh();   // 新建自定义网格
//    QOpenGLFunctions *function = QOpenGLContext::currentContext()->functions();
//    cmesh->m_vao = new QOpenGLVertexArrayObject();

//    for(int i = 0; i < scene->mNumMeshes; i++)
//    {
//        qDebug()<< "read mesh: "<< i;
//        aiMesh* mesh = meshes[i];
//        qDebug() << "faces: " << mesh->HasFaces() << mesh->mNumFaces
//                 << "position: " << mesh->HasPositions()
//                 << "vertices: " << mesh->mNumVertices
//                 << "textureCoords: " << mesh->HasTextureCoords(0) << mesh->HasTextureCoords(1)
//                 << "hasVertexColors: " << mesh->HasVertexColors(0) << mesh->HasVertexColors(1)
//                 << "material index: " << mesh->mMaterialIndex;

//    }

//    aiMaterial ** mats = scene->mMaterials; // materials

//    for(int i = 0; i < scene->mNumMaterials; i++)
//    {
//        qDebug() << "read materials: "<< i;
//        aiMaterial * mat = mats[i];
//        qDebug() << "numproperties: " << mat->mNumProperties;
//    }

//    return NULL;
//}

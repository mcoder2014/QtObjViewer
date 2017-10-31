#include "objLoader.h"
#include <QDebug>

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
        aiMesh* mesh = meshes[i];
        qDebug() << "faces: " << mesh->HasFaces() << mesh->mNumFaces
                 << "position: " << mesh->HasPositions()
                 << "vertices: " << mesh->mNumVertices
                 << "textureCoords: " << mesh->HasTextureCoords(0) << mesh->HasTextureCoords(1)
                 << "hasVertexColors: " << mesh->HasVertexColors(0) << mesh->HasVertexColors(1)
                 << "material index: " << mesh->mMaterialIndex;

    }

    aiMaterial ** mats = scene->mMaterials; // materials

    for(int i = 0; i < scene->mNumMaterials; i++)
    {
        qDebug() << "read materials: "<< i;
        aiMaterial * mat = mats[i];
        qDebug() << "numproperties: " << mat->mNumProperties;
    }

}

CustomMesh *objLoader::getMesh(const aiScene *scene)
{
    if(scene == NULL)
    {
        return;
    }
    aiMesh ** meshes = scene->mMeshes;      // meshes数组

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        qDebug()<< "read mesh: "<< i;
        aiMesh* mesh = meshes[i];
        qDebug() << "faces: " << mesh->HasFaces() << mesh->mNumFaces
                 << "position: " << mesh->HasPositions()
                 << "vertices: " << mesh->mNumVertices
                 << "textureCoords: " << mesh->HasTextureCoords(0) << mesh->HasTextureCoords(1)
                 << "hasVertexColors: " << mesh->HasVertexColors(0) << mesh->HasVertexColors(1)
                 << "material index: " << mesh->mMaterialIndex;

    }

    aiMaterial ** mats = scene->mMaterials; // materials

    for(int i = 0; i < scene->mNumMaterials; i++)
    {
        qDebug() << "read materials: "<< i;
        aiMaterial * mat = mats[i];
        qDebug() << "numproperties: " << mat->mNumProperties;
    }
}

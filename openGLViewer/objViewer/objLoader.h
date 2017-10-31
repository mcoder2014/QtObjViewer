#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QObject>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

class CustomMesh;

class objLoader : public QObject
{
    Q_OBJECT
public:
    explicit objLoader(QObject *parent = nullptr);
    const aiScene *scene;       // scene
    const aiScene *loadFile(QString filePath);
    void readAiScene(const aiScene *scene);
    CustomMesh *getMesh(const aiScene *scene);
};

#endif // OBJLOADER_H

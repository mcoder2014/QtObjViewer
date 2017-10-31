#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QObject>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

#include "CustomMesh.h"

class objLoader : public QObject
{
    Q_OBJECT
public:
    explicit objLoader(QObject *parent = nullptr);
    const aiScene *scene;

public slots:
    const aiScene *loadFile(QString filePath);
    void readAiScene(const aiScene *scene);
    CustomMesh *getMesh(const aiScene *scene);

signals:

};

#endif // OBJLOADER_H

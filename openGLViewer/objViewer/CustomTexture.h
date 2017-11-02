#ifndef CUSTOMTEXTURE_H
#define CUSTOMTEXTURE_H

#include <assimp/scene.h>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class CustomTexture
        :public QObject
{
    Q_OBJECT
public:
    CustomTexture(QObject *parent = 0);
//    GLuint id;
    aiTexture type;         // 图片
    QString path;           // 文件路径
    QOpenGLTexture *texture;
    bool isLoaded;          // 是否加载了图片的标签

    void load2DTextrue(QString fileName);

};

#endif // CUSTOMTEXTURE_H

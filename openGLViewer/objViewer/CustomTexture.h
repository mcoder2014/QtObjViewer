#ifndef CUSTOMTEXTURE_H
#define CUSTOMTEXTURE_H

#include <assimp/scene.h>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class CustomTexture
{
public:
    CustomTexture();
//    GLuint id;
    aiTexture type;         // 图片
    QString path;           // 文件路径

};

#endif // CUSTOMTEXTURE_H

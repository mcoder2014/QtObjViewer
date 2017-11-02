#include "CustomTexture.h"
#include <QOpenGLFunctions>

CustomTexture::CustomTexture(QObject *parent)
    :QObject()
{
    this->texture = NULL;
    this->isLoaded = false;
}

void CustomTexture::load2DTextrue(QString fileName)
{
    QImage image(fileName);         // 加载一个图片
    if(this->texture == NULL)
    {
        this->texture = new QOpenGLTexture(image);
    }
    this->path = fileName;          // 记录路径

    this->texture->create();        // 创建
//    this->texture->setData(image.mirrored());  // 设置图片
    this->texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
    this->isLoaded = true;
}

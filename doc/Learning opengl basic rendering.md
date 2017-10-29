# 关于Qt 5 提供的关于OpenGL的数据类型

## QOpenGLBuffer
`QOpenGLBuffer`类似一种一组特殊的 id来管理GPU的动态内存。它比较灵活你可以简单的理解为它负责GPU的动态内存管理，你可以告诉GPU你准备如何使用内存空间。Qt5提供`QOpenGLBuffer`作为工具类包含了关于`OpenGL Buffers`相关的函数。

## QOpenGLVertexArrayObject
主要是负责存储在GPU上的点的信息，这样就不用像纯OpenGL程序那样 不停地bind unbind了。

## QOpenGLShaderProgram
一段运行在GPU上的代码。
![](images\ShaderPipeline1.png)

shder有很多部分，我们主要写点着色器和片元着色器

# References
1. [Qt5 OpenGL Part 1: Basic Rendering ](http://www.trentreed.net/blog/qt5-opengl-part-1-basic-rendering/)

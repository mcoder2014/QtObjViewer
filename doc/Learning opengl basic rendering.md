# 关于Qt 5 提供的关于OpenGL的数据类型

## QOpenGLBuffer
`QOpenGLBuffer`类似一种一组特殊的 id来管理GPU的动态内存。它比较灵活你可以简单的理解为它负责GPU的动态内存管理，你可以告诉GPU你准备如何使用内存空间。Qt5提供`QOpenGLBuffer`作为工具类包含了关于`OpenGL Buffers`相关的函数。

## QOpenGLVertexArrayObject
主要是负责存储在GPU上的点的信息，这样就不用像纯OpenGL程序那样 不停地bind unbind了。

## QOpenGLShaderProgram
一段运行在GPU上的代码。
![](images\ShaderPipeline1.png)

shder有很多部分，我们主要写点着色器和片元着色器.

- Vertex Shader
> Takes input information (usually in the form of vertex information) and processes the information to find final positions that the GPU must draw and interpolate between. Interpolation will be handled automatically by the device in-between the Vertex Shader and Fragment Shader. The Vertex information we will use is: position, and color.

- Fragment Shader
> Takes the interpolated results from the Vertex Shader, and outputs calculations somewhere. Right now, that sounds pretty mysterious, but the base case for a Fragment Shader is to output to the back buffer, so that we can call our system-specific SwapBuffer() command to make it visible.
Note: There is some confusion about whether to call this a Fragment shader or Pixel shader. In my opinion, both names are sufficient, and any graphics programmer will know what you mean when you use one name or the other. I prefer OpenGL terminology because the Fragment shader describes what happens to a group of outputs represented by the transformed vertex inputs. (e.g. we don’t output to a “pixel”, and we evaluate many “outputs”)

# References
1. [Qt5 OpenGL Part 1: Basic Rendering ](http://www.trentreed.net/blog/qt5-opengl-part-1-basic-rendering/)
2. [Unity Shader入门精要](https://book.douban.com/subject/26821639/?from=tag)

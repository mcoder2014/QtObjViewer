#version 330
//layout(location = 0) in vec3 position;      # 点的集合坐标
//layout(location = 1) in vec3 normal;        # 点的法向量
//layout(location = 2) in vec2 textureCoord;      # UV 坐标

//out vec4 vColor;
//out vec2 texCoord;

//uniform mat4 modelToWorld;
//uniform mat4 worldToCamera;
//uniform mat4 cameraToView;

//void main()
//{
//  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
//  vColor = vec4(normal, 1.0);
////  texCoord = textureCoord;

//}

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

out vec4 vColor;
out vec2 texCoord;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

void main()
{
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
  vColor = vec4(normal, 1.0);
//  vColor = vec4(textureCoord,1.0,1.0);
//  vColor = vec4(position,1.0);
  texCoord = textureCoord;
}

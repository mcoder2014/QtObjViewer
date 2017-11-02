#version 330 core
//in highp vec4 vColor;
////in highp vec2 texCoord;
//out highp vec4 fColor;

//void main()
//{

////    fColor = vec4(1.0f,1.0f,1.0f,1.0f);
//    fColor = vColor;
//}
in highp vec4 vColor;
in highp vec2 texCoord;
out highp vec4 fColor;

uniform sampler2D texture_diffuse;

void main()
{
//   fColor = vColor;
    fColor = texture(texture_diffuse, texCoord);
}

#version 330
in highp vec4 vColor;
in highp vec2 texCoord;
out highp vec4 fColor;

void main()
{

//    fColor = vec4(1.0f,1.0f,1.0f,1.0f);
    fColor = vColor;
}

#version 460 core
layout(location=0)in vec3 pos;
layout(location=1)in vec2 uv;
layout(std140)uniform VP
{
mat4 view;
mat4 projection;
mat4 vp;
};
/////////////////////////
out vec2 TexCoord;
out vec3 FragPos;
out float Alpha;
/////////////////////////
void main()
{
TexCoord=uv;
FragPos=pos;
gl_Position=vp*vec4(pos,1.0);
}
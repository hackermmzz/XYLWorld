#version 460 core
layout(location=0)in vec3 pos;
layout(std140)uniform VP
{
mat4 view;
mat4 projection;
mat4 vp;
};
void main(){
	gl_Position=vp*vec4(pos,1.0);
}
#version 460 core
layout(location=0)in vec3 pos;
layout(std140)uniform VP
{
mat4 view;
mat4 projection;
mat4 vp;
};
uniform vec3 Model;
uniform vec3 RolePos;
out vec3 color;
void main(){
	gl_Position=vp*vec4(pos*Model+RolePos,1.0);
	color=pos*0.5+1.0;
}
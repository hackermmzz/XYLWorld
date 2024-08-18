#version 460 core
layout(location=0)in vec3 vertex;
uniform mat4 vp;
out vec3 TexCoord;
void main(){
	vec4 pos=vp*vec4(vertex,1.0);
	TexCoord=vertex;
	gl_Position=pos.xyww;
}
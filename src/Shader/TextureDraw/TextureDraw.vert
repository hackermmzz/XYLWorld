#version 330 core 
layout(location=0)in vec2 vertex;
layout(location=1)in vec2 TexCoord_;
uniform vec2 offset;
uniform vec2 scale;
out vec2 TexCoord;
void main(){
	TexCoord=TexCoord_*scale+offset;
	gl_Position=vec4(vertex,0.0,1.0);
}
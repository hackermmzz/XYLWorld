#version 460 core
layout(location=0)in vec2 pos;
uniform float width;
uniform float height;
void main(){
	gl_Position=vec4(pos.x/width,pos.y/height,0.0,1.0);
}
#version 330 core
uniform sampler2D sampler;
in vec2 TexCoord;
out vec4 color;
void main(){
	color=texture2D(sampler,TexCoord);
}
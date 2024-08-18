#version 330 core
in vec2 UV;
uniform sampler2D sampler;
uniform float Brightness;
out vec4 color;
void main(){
	color=vec4(texture(sampler,UV).rgb*Brightness,1.0);
}
#version 460 core
uniform sampler2D sampler;
out vec4 color;
in vec2 TexCoord;
in float FrogFac;
void main(){
	color=texture(sampler,TexCoord);
	if(color.a==0.0)discard;
	color+=vec4(1.0)*FrogFac;
}
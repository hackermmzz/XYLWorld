#version 460 core
in vec2 TexCoord;
in vec3 FragPos;
out vec4 color;
//////////////////////////
uniform sampler2D wave0;
uniform sampler2D wave1;
uniform sampler2D water;
uniform float time;
uniform vec3 camera;
void main()
{
//////////////////////////////
color=vec4(texture(water,TexCoord).rgb,0.7);
}
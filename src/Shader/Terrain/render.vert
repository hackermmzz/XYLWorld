#version 460 core
layout(location=0)in vec3 pos;
layout(location=1)in vec2 texcoord;
layout(std140)uniform VP
{
mat4 view;
mat4 projection;
mat4 vp;
};
uniform vec3 Eye;
const float FrogMaxDis=400.0;
const float FrogMinDis=250.0;
const float FrogWeight=FrogMaxDis-FrogMinDis;
const vec2 color[4]={vec2(0.0),vec2(1.0,0.0),vec2(1.0),vec2(0.0,1.0)};
out vec2 TexCoord;
out float FrogFac;
void main(){
	TexCoord=texcoord;
	gl_Position=vp*vec4(pos,1.0);
	float DistanceToEye=length(Eye-pos);
	FrogFac=(clamp(DistanceToEye,FrogMinDis,FrogMaxDis)-FrogMinDis)/FrogWeight;
	FrogFac=smoothstep(0.0,1.0,FrogFac);
}
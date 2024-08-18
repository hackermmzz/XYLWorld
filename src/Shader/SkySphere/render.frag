#version 460 core
const float PI=acos(-1.0);
const vec3 SkyColor=vec3(0.0,0.633,0.91);
const float SunRadRangeToDegree=10.0;
const float SunRadRangeToRadian=SunRadRangeToDegree*PI/180.0;
const vec3 SunColor0=vec3(1.0,1.0,1.0);
const vec3 SunColor1=vec3(1.0,0.3,0.4);
uniform sampler2D Cloud;
uniform sampler2D Star;
uniform vec3 SunPos;
uniform float Brightness;
in vec3 TexCoord;
out vec4 color;
void main(){
    float RealBrightness=clamp(Brightness,0.0,1.0);
 //将球面坐标映射到纹理坐标
	float theta = acos(TexCoord.z);
    float phi = atan(abs(TexCoord.y), TexCoord.x);
    float s = (phi) / (2.0 *PI);
    float t = theta/PI;
    vec2 uv=vec2(s,t);
    //得到此片段云的颜色，对越接近y=-1与y=1的点淡化
    float fac=texture(Cloud,uv).r;
    float fac1=smoothstep(0.0,0.3,abs(TexCoord.y));
    fac*=fac1;
    color=vec4(mix(SkyColor,vec3(1.0),fac),1.0)*RealBrightness;
    //得到此片段太阳的颜色
    if(Brightness>0.0){
        float degree=acos(dot(SunPos,TexCoord));
        degree=SunRadRangeToRadian-degree;
        degree=clamp(degree,0.0,SunRadRangeToRadian);
        float fac2=smoothstep(0.0,SunRadRangeToRadian,degree);
        vec3 SunColor=mix(SunColor0,SunColor1,fac2);
        color=vec4(mix(color.rgb,SunColor,fac2),1.0);   
    }
    //绘制星星的颜色
    else{
        vec3 starColor=texture(Star,uv).rgb*abs(Brightness);
        color=vec4(starColor,1.0);
    }
}
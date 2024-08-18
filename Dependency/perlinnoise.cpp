#include "perlinnoise.h"
/////////////////////////////////////
const int DefaultPermutationTable[]= { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
};
/////////////////////////////////////
double PerlinNoise3D(int*permutation,double x, double y, double z)
{
    //随机向量的获取
    auto Grad=[&](int hash,double x,double y,double z)
    {
       switch(hash & 0xF)
       {
          case 0x0: return  x + y;
          case 0x1: return -x + y;
          case 0x2: return  x - y;
          case 0x3: return -x - y;
          case 0x4: return  x + z;
          case 0x5: return -x + z;
          case 0x6: return  x - z;
          case 0x7: return -x - z;
          case 0x8: return  y + z;
          case 0x9: return -y + z;
          case 0xA: return  y - z;
          case 0xB: return -y - z;
          case 0xC: return  y + x;
          case 0xD: return -y + z;
          case 0xE: return  y - x;
          case 0xF: return -y - z;
        }
       return 0.0;
    };
    //开始计算
    int xx=floor(x),yy=floor(y),zz=floor(z);
    double xf=x-xx,yf=y-yy,zf=z-zz;
    double u=Fade(xf),v=Fade(yf),w=Fade(zf);
    int xi=xx&255,yi=yy&255,zi=zz&255;
    //获取随机索引
    int hash0,hash1,hash2,hash3,hash4,hash5,hash6,hash7;
    hash0=permutation[permutation[permutation[xi]+yi]+zi];
    hash1=permutation[permutation[permutation[xi+1]+yi]+zi];
    hash2=permutation[permutation[permutation[xi]+yi+1]+zi];
    hash3=permutation[permutation[permutation[xi]+yi]+zi+1];
    hash4=permutation[permutation[permutation[xi+1]+yi+1]+zi];
    hash5=permutation[permutation[permutation[xi+1]+yi]+zi+1];
    hash6=permutation[permutation[permutation[xi]+yi+1]+zi+1];
    hash7=permutation[permutation[permutation[xi+1]+yi+1]+zi+1];
    //线性插值
    double x1,x2,y1,y2;
    x1=Lerp(Grad(hash0,xf,yf,zf),Grad(hash1,xf-1,yf,zf),u);
    x2=Lerp(Grad(hash2,xf,yf-1,zf),Grad(hash3,xf-1,yf-1,zf),u);
    y1=Lerp(x1,x2,v);
    x1=Lerp(Grad (hash4,xf,yf,zf-1),Grad(hash5,xf-1,yf,zf-1), u);
    x2=Lerp(Grad (hash6, xf  , yf-1, zf-1),Grad (hash7, xf-1, yf-1, zf-1),u);
    y2=Lerp(x1,x2,v);
    return Lerp(y1,y2,w);
}

double PerlinNoise3D(int*permutation,double x, double y, double z, uint octaves, double persistence)
{
    double res=0;
    double zf=1.0;
    for(uint i=0;i<octaves;++i)
    {
        res+=PerlinNoise3D(permutation,x,y,z)*zf;
        zf*=persistence;
        x*=2.0;
        y*=2.0;
        z*=2.0;
    }
    return ClampTo11(res);
}

PerlinNoiseGenerator::PerlinNoiseGenerator(int seed)
{
    unsigned long long S=seed;
    static unsigned long long MulFac=20041118ull*20050119ull;
    permutationTable.resize(512);
    vector<int>v(256);
    iota(v.begin(),v.end(),0);
    for(int i=0;i<256;++i)
    {
        S=S*MulFac;
        int idx=S%v.size();
        swap(v[idx],v.back());
        permutationTable[i]=permutationTable[i+256]=DefaultPermutationTable[v.back()];
        v.pop_back();
    }
}

double PerlinNoiseGenerator::generate01(double x, double y, double z)
{
    return generate11(x,y,z)*0.5+0.5;
}

double PerlinNoiseGenerator::generate01(double x, double y)
{
    return generate11(x,y)*0.5+0.5;
}

double PerlinNoiseGenerator::generate01(double x, double y, uint octaves, double persistence)
{
    return PerlinNoise2D(permutationTable.data(),x,y,octaves,persistence)*0.5+0.5;
}

double PerlinNoiseGenerator::generate01(double x, double y, double z, uint octaves, double persistence)
{
    return PerlinNoise3D(permutationTable.data(),x,y,z,octaves,persistence)*0.5+0.5;
}

double PerlinNoiseGenerator::generate11(double x, double y, double z)
{
    return PerlinNoise3D(permutationTable.data(),x,y,z);
}

double PerlinNoiseGenerator::generate11(double x, double y)
{
    return PerlinNoise2D(permutationTable.data(),x,y);
}

double PerlinNoiseGenerator::generate11(double x, double y, uint octaves, double persistence)
{
    return PerlinNoise2D(permutationTable.data(),x,y,octaves,persistence);
}

double PerlinNoiseGenerator::generate11(double x, double y, double z, uint octaves, double persistence)
{
    return PerlinNoise3D(permutationTable.data(),x,y,z,octaves,persistence);
}

double PerlinNoise2D(int *permutation, double x, double y)
{
    //随机向量的获取
    auto Grad=[&](int hash,double x,double y)
    {
       switch(hash & 0x3)
       {
          case 0x0: return  x + y;
          case 0x1: return -x + y;
          case 0x2: return  x - y;
          case 0x3: return -x - y;
        }
       return 0.0;
    };
    //开始计算
    int xx=floor(x),yy=floor(y);
    double xf=x-xx,yf=y-yy;
    double u=Fade(xf),v=Fade(yf);
    int xi=xx&255,yi=yy&255;
    //获取随机索引
    int hash0,hash1,hash2,hash3;
    hash0=permutation[permutation[xi]+yi];
    hash1=permutation[permutation[xi+1]+yi];
    hash2=permutation[permutation[xi]+yi+1];
    hash3=permutation[permutation[xi+1]+yi+1];
    //线性插值
    double x1,x2,res;
    x1=Lerp(Grad(hash0,xf,yf),Grad(hash1,xf-1,yf),u);
    x2=Lerp(Grad(hash2,xf,yf-1),Grad(hash3,xf-1,yf-1),u);
    return Lerp(x1,x2,v);
}

double PerlinNoise2D(int *permutation, double x, double y, uint octaves, double persistence)
{
    double res=0;
    double zf=1.0;
    for(uint i=0;i<octaves;++i)
    {
        res+=PerlinNoise2D(permutation,x,y)*zf;
        zf*=persistence;
        x*=2.0;
        y*=2.0;
    }
     return ClampTo11(res);
}

double Fade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double Lerp(double a, double b, double f){
    return a+(b-a)*f;
}

double ClampTo11(double r)
{
    if(r<=-1.0)return -1.0;
    if(r>=1.0)return 1.0;
    return r;
}

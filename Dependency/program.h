#ifndef PROGRAM_H
#define PROGRAM_H
#include"include.h"
#include"qfile.h"
#include"qdir.h"
#include"ubo.h"
#include"ssbo.h"
struct  Program
{
    GLuint id;
    /////////////////////
    Program(const char*vertexFile,const char*fragmentFile);
    Program(const char*vertexFile,const char*geomeryFile,const char*fragmentFile,const vector<string>&varyings);
    void bind();
    int GetLocation(const char*name);
    void setFloat(const char*name,float val);
    void setVec2(const char*name,const vec2&val);
    void setVec3(const char*name,const vec3&val );
    void setUint(const char*name,uint val);
    void setInt(const char*name,int val);
    void setUbo(const char*name,uint idx);
    void setSsbo(const char*name,uint idx);
    void setMat4(const char*name,const mat4&mat);
    ~Program();
};

#endif // PROGRAM_H

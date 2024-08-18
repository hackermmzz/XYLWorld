#include "program.h"

Program::Program(const char *vertexFile, const char *fragmentFile)
    :Program(vertexFile,0,fragmentFile,{})
{

}

Program::Program(const char *vertexFile, const char *geomeryFile, const char *fragmentFile, const vector<string> &varyings)
{
    auto ReadFile=[&](const char*file)->string{
        if(file==0)return "";
        QFile f(file);
        if(f.open(QIODevice::ReadOnly)==0)
        {
            qDebug()<<file<<" not exist!";
            return "";
        }
        string res;
        res.resize(f.size());
        f.read(&res[0],res.size());
        return res;
    };
    auto GetShaderError=[&](GLuint id,const char*file)->void{
         if(file==0)return;
         int success;
          api->glGetShaderiv(id, GL_COMPILE_STATUS, &success);
          if (success)return;
          QString SaveToFilePath=QDir::currentPath()+QString("/ShaderError.txt");
          QFile f(SaveToFilePath);
          f.open(QIODevice::Append);
          char*info=new char[1024*1024*2+1];//2MB
          api->glGetShaderInfoLog(id, 1024*1024*2, 0, info);
          string res(info);
          res+="\n+++++++++++++++++++++++++++++++++++\n";
          f.write(res.data(),res.size());
          delete[]info;
    };
    auto GetProgramError=[&](GLuint id){
          int status = 0;
          api->glGetProgramiv(id, GL_LINK_STATUS, &status);
          if (status == GL_FALSE) {
          QString SaveToFilePath=QDir::currentPath()+QString("/ShaderError.txt");
          QFile f(SaveToFilePath);
          f.open(QIODevice::Append);
          char *info=new char[1024*1024*2+1];
          api->glGetProgramInfoLog(id, 1024*1024, 0, info);
          string res(info);
          res+="Program\n++++++++++++++++++++++++++++++\n";
          f.write(res.data(),res.size());
          f.close();
          delete []info;
           }
      };
    auto CompileShader=[&](GLenum type,const string&code,const char*file)->GLuint{
        if(file==0)return -1;
           auto id=api->glCreateShader(type);
           auto*data=code.data();
           api->glShaderSource(id,1,&data,0);
           api->glCompileShader(id);
           GetShaderError(id,file);
           return id;
       };
    auto id0=CompileShader(GL_VERTEX_SHADER,ReadFile(vertexFile),vertexFile);
    auto id1=CompileShader(GL_GEOMETRY_SHADER,ReadFile(geomeryFile),geomeryFile);
    auto id2=CompileShader(GL_FRAGMENT_SHADER,ReadFile(fragmentFile),fragmentFile);
    id= api->glCreateProgram();
    if(id0!=-1)api->glAttachShader(id,id0);
    if(id1!=-1)api->glAttachShader(id,id1);
    if(id2!=-1)api->glAttachShader(id,id2);
    if(varyings.size()){
       vector<const char*>v;
       for(auto&s:varyings)v.push_back(s.c_str());
        api->glTransformFeedbackVaryings(id, varyings.size(),v.data(), GL_INTERLEAVED_ATTRIBS);
    }
    api->glLinkProgram(id);
    GetProgramError(id);
    if(id0!=-1)api->glDeleteShader(id0);
    if(id1!=-1)api->glDeleteShader(id1);
    if(id2!=-1)api->glDeleteShader(id2);
    ////////////
    bind();
}

void Program::bind()
{
    api->glUseProgram(id);
}

int Program::GetLocation(const char*name)
{
    int location = api->glGetUniformLocation(id, name);
    if(location<0)qDebug()<<name<<"can't be located!";
    return location;
}

void Program::setFloat(const char *name, float val)
{
    api->glUniform1f(GetLocation(name),val);
}

void Program::setVec2(const char *name, const vec2 &val)
{
    api->glUniform2fv(GetLocation(name),1,(GLfloat*)&val);
}

void Program::setVec3(const char *name, const vec3 &val)
{
    api->glUniform3fv(GetLocation(name),1,(GLfloat*)&val);
}

void Program::setUint(const char *name, uint val)
{
    api->glUniform1ui(GetLocation(name),val);
}

void Program::setInt(const char *name, int val)
{
    api->glUniform1i(GetLocation(name),val);
}

void Program::setUbo(const char *name, uint idx)
{
    int location =api->glGetUniformBlockIndex(id, name);
    if (location < 0)
    {
        qDebug()<<name<<" can't be located!";
        return;
    }
    api->glUniformBlockBinding(id, location, idx);
}

void Program::setSsbo(const char *name, uint idx)
{
    GLuint location = api->glGetProgramResourceIndex(id,GL_SHADER_STORAGE_BLOCK, name);
    if (location < 0){
        qDebug()<<name<<" can't be located!";
        return;
    }
    api->glShaderStorageBlockBinding(id,location,idx);
}

void Program::setMat4(const char *name, const mat4 &mat)
{
      api->glUniformMatrix4fv(GetLocation(name), 1,0,value_ptr(mat));
}

Program::~Program()
{
    if(id)api->glDeleteProgram(id);
}

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include"include.h"
#include"texture2dgeneral.h"
#include"vbo.h"
#include"vao.h"
#include"program.h"
using std::vector;
using std::string;
/////////////////////////////////////
class TransformFeedBack {
private:
    GLuint id;
public:
    TransformFeedBack();
    void bind();
    void BindTransformBuffer(GLuint id,GLuint binding=0);
    void Begin(GLenum mode=GL_POINTS);
    void End();
    GLuint getid();
    ~TransformFeedBack();
};
//////////////////////////////////////
class ParticleSystem
{
private:
    TransformFeedBack* tfb[2];
    Vao* vao[2];
    Vbo* buffer[2];
    int index;
    bool init;
    GLuint LaunchersNum;
public:
    ParticleSystem(Vao*vao[2],Vbo* buffer[2],GLuint LaunchersNum);
    void Reset();
    void Update(GLenum mode=GL_POINTS);
    Vbo* GetCurrentRenderBuffer();
    void Draw(GLenum mode=GL_POINTS);
    ~ParticleSystem();
};
#endif // PARTICLESYSTEM_H

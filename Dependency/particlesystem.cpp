#include "particlesystem.h"
TransformFeedBack::TransformFeedBack()
{
    api->glGenTransformFeedbacks(1, &id);
    bind();
}

void TransformFeedBack::bind()
{
    api->glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, id);
}

void TransformFeedBack::BindTransformBuffer(GLuint id, GLuint binding)
{
    api->glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding,id);
}

void TransformFeedBack::Begin(GLenum mode)
{
    api->glEnable(GL_RASTERIZER_DISCARD);
    api->glBeginTransformFeedback(mode);
}

void TransformFeedBack::End()
{
    api->glEndTransformFeedback();
    api->glDisable(GL_RASTERIZER_DISCARD);
}

GLuint TransformFeedBack::getid()
{
    return id;
}

TransformFeedBack::~TransformFeedBack()
{
    if (id)api->glDeleteTransformFeedbacks(1, &id);
}


ParticleSystem::ParticleSystem(Vao* vao_[2], Vbo* buffer_[2], GLuint LaunchersNum_)
{
    LaunchersNum = LaunchersNum_;
    Reset();
    ////////////////////////////
    for (int i = 0; i < 2; ++i)
    {
        vao[i] = vao_[i];
        buffer[i] = buffer_[i];
        tfb[i] = new TransformFeedBack;
        tfb[i]->BindTransformBuffer(buffer_[(i+1)&1]->id, 0);
    }
}

void ParticleSystem::Reset()
{
    init=0;
    index=0;
}

void ParticleSystem::Update(GLenum mode)
{
    auto& tfb_ =*tfb[index];
    tfb_.bind();
    tfb_.Begin();
    vao[index]->bind();
    if (!init)
    {
        init = 1;
        api->glDrawArrays(mode, 0, LaunchersNum);
    }
    else
        Draw(mode);
    index = (index + 1) & 1;
    tfb_.End();
}

Vbo* ParticleSystem::GetCurrentRenderBuffer()
{
    return buffer[index];
}

void ParticleSystem::Draw(GLenum mode)
{
    api->glDrawTransformFeedback(mode, tfb[(index+1)&1]->getid());
}

ParticleSystem::~ParticleSystem()
{
    for (auto& tfb_ : tfb)
        if (tfb_)delete tfb_;
}

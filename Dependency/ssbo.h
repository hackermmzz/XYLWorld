#ifndef SSBO_H
#define SSBO_H
#include"bufferbase.hpp"
struct Ssbo:BufferBase<GL_SHADER_STORAGE_BUFFER>
{
    //////////////////////
    Ssbo(GLuint size,void*data);
    void bindBase(GLuint idx);
};

#endif // SSBO_H

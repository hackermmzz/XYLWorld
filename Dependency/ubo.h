#ifndef UBO_H
#define UBO_H
#include"bufferbase.hpp"
struct Ubo:BufferBase<GL_UNIFORM_BUFFER>
{
    ///////////////////
    Ubo(GLuint size,void*data);
    void bindBase(GLuint index);
};

#endif // UBO_H

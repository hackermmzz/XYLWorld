#ifndef EBO_H
#define EBO_H
#include"bufferbase.hpp"
struct Ebo:BufferBase<GL_ELEMENT_ARRAY_BUFFER>
{
public:
    Ebo(GLuint count,void*data);
};

#endif // EBO_H

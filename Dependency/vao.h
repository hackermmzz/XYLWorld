#ifndef VAO_H
#define VAO_H
#include"include.h"
struct Vao
{
    GLuint id;
    /////////////////////
    Vao();
    void bind();
    void unbind();
    ~Vao();
};

#endif // VAO_H

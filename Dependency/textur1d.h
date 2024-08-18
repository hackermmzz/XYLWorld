#ifndef TEXTUR1D_H
#define TEXTUR1D_H
#include"include.h"
struct  Texture1D {
    GLuint id;
    ///////////////////////
    Texture1D(GLuint size, GLenum internalformat, GLenum format, GLenum type, void* data=0);
    void bind(GLuint binding = 0);
    void wrap(GLenum type);
    void filter(GLenum type);
    ~Texture1D();
};
#endif // TEXTUR1D_H

#ifndef VBO_H
#define VBO_H
#include"bufferbase.hpp"
struct Vbo:BufferBase<GL_ARRAY_BUFFER>
{
    ///////////////////////
    Vbo(GLuint size,void*data);
    void setpoint(GLuint location, GLuint divisor);
    void setpoint(GLuint location, GLenum value_type, GLuint num, GLuint step_size, GLuint offset);
};

#endif // VBO_H

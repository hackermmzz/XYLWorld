#include "ebo.h"

Ebo::Ebo(GLuint count, void *data):BufferBase<GL_ELEMENT_ARRAY_BUFFER>(count*sizeof(GLuint),data)
{

}

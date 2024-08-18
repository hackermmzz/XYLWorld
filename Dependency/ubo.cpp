#include "ubo.h"

Ubo::Ubo(GLuint size, void *data):Ubo::TYPE(size,data)
{

}

void Ubo::bindBase(GLuint index)
{
    api->glBindBufferBase(GL_UNIFORM_BUFFER,index,id);
}

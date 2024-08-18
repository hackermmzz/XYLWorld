#include "ssbo.h"

Ssbo::Ssbo(GLuint size,void*data):Ssbo::TYPE(size,data)
{

}

void Ssbo::bindBase(GLuint idx)
{
    api->glBindBufferBase(GL_SHADER_STORAGE_BUFFER,idx,id);
}

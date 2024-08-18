#include "vao.h"

Vao::Vao()
{
    api->glGenVertexArrays(1,&id);
    bind();
}

void Vao::bind()
{
    api->glBindVertexArray(id);
}

void Vao::unbind()
{
    api->glBindVertexArray(0);
}

Vao::~Vao()
{
    if(id)api->glDeleteVertexArrays(1,&id);
}

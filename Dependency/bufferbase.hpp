#ifndef BUFFERBASE_H
#define BUFFERBASE_H
#include"include.h"
template<GLenum Type>
struct BufferBase
{
    GLuint id;
    GLuint size;
    using TYPE=BufferBase<Type>;
    ////////////////////////////////
    BufferBase(GLuint size,void*data=0);
    void setdata(GLuint offset, GLuint size, void *data);
    void bind();
    void unbind();
    void *map(GLuint offset,GLuint size,GLuint mask=GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    void unmap();
    virtual ~BufferBase();
};
/////////////////////////////////////////////


template<GLenum Type>
BufferBase<Type>::BufferBase(GLuint size_, void *data)
{
    size=size_;
    api->glGenBuffers(1,&id);
    bind();
    api->glBufferData(Type,size,data,GL_STATIC_DRAW);
}

template<GLenum Type>
void BufferBase<Type>::setdata(GLuint offset, GLuint size, void *data)
{
     api->glBufferSubData(Type, offset, size, data);
}

template<GLenum Type>
void BufferBase<Type>::bind()
{
    api->glBindBuffer(Type,id);
}

template<GLenum Type>
void BufferBase<Type>::unbind()
{
    api->glBindBuffer(Type,0);
}

template<GLenum Type>
void *BufferBase<Type>::map(GLuint offset, GLuint size, GLuint mask)
{
    return api->glMapBufferRange(Type,offset,size,mask);
}

template<GLenum Type>
void BufferBase<Type>::unmap()
{
    api->glUnmapBuffer(Type);
}

template<GLenum Type>
BufferBase<Type>::~BufferBase()
{
    if(id)
    api->glDeleteBuffers(1,&id);
}
#endif // BUFFERBASE_H

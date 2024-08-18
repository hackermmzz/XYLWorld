#include "texture2d.h"


Texture2D::Texture2D(const string &file)
{
    api->glGenTextures(1,&id);
    api->glBindTexture(GL_TEXTURE_2D,id);
    auto img=QImage(file.c_str()).convertToFormat(QImage::Format_RGBA8888);
    width=img.width(),height=img.height();
    api->glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,img.bits());
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    api->glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::Texture2D(GLuint w, GLuint h, void *data)
{
    api->glGenTextures(1,&id);
    api->glBindTexture(GL_TEXTURE_2D,id);
    width=w,height=h;
    api->glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    api->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    api->glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture2D::bind(GLuint idx)
{
    api->glActiveTexture(GL_TEXTURE0+idx);
    api->glBindTexture(GL_TEXTURE_2D,id);
}

void Texture2D::draw(GLfloat xbegin,GLfloat ybegin,GLfloat width,GLfloat height)
{
    static Program*p=0;
    static Vao*vao;
    if(!p)
    {
        p=new Program("src/Shader/TextureDraw/TextureDraw.vert","src/Shader/TextureDraw/TextureDraw.frag");
        vao=new Vao();
        GLfloat vertex[][4]={{-1,1,0,0},{1,1,1,0},{1,-1,1,1},{-1,-1,0,1}};
        GLuint indices[]={0,1,2,0,2,3};
        auto*vbo=new Vbo(sizeof(vertex),vertex);
        vbo->setpoint(0,GL_FLOAT,2,sizeof(float)*4,0);
        vbo->setpoint(1,GL_FLOAT,2,sizeof(float)*4,sizeof(float)*2);
        auto*ebo=new Ebo(6,indices);
        vao->unbind();
    }
    vao->bind();
    p->bind();
    p->setUint("sampler",0);
    p->setVec2("offset",vec2{xbegin,ybegin});
    p->setVec2("scale",vec2{width,height});
    bind(0);
    api->glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
}

void Texture2D::DrawTexture2D(GLuint id,float Brightness)
{
    static Vao*vao;
    static Vbo*vbo;
    static Ebo*ebo;
    static bool init=0;
    static Program*program;
    if(!init)
    {
        init=1;
        vao=new Vao();
        GLfloat vertex[]{
            -1,1,0,1,
            1,1,1,1,
            -1,-1,0,0,
            1,-1,1,0
        };
        GLuint index[]{0,1,2,1,2,3};
        vbo=new Vbo(sizeof(vertex),vertex);
        vbo->setpoint(0,GL_FLOAT,2,sizeof(float)*4,0);
        vbo->setpoint(1,GL_FLOAT,2,sizeof(float)*4,sizeof(float)*2);
        ebo=new Ebo(6,index);
        vao->unbind();
        /////////////////////
        program=new Program("src/Shader/TextureDraw/TextureDraw1.vert","src/Shader/TextureDraw/TextureDraw1.frag");
        /////////////////////
    }
    vao->bind();
    if(id!=0)
    {
        program->bind();
        program->setFloat("Brightness",Brightness);
        api->glActiveTexture(GL_TEXTURE0);
        api->glBindTexture(GL_TEXTURE_2D,id);
    }
    api->glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
}

Texture2D::~Texture2D()
{
    if(id)
        api->glDeleteTextures(1,&id);
}

#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include"include.h"
#include"string"
#include"qimage.h"
#include"program.h"
#include"vao.h"
#include"vbo.h"
#include"ebo.h"
using namespace std;
struct Texture2D
{
    GLuint id;
    GLuint width,height;
    ///////////////////////////
    Texture2D(const string&file);
    Texture2D(GLuint w,GLuint h,void*data);
    void bind(GLuint idx);
    void draw(GLfloat xbegin=0,GLfloat ybegin=0,GLfloat width=1.0,GLfloat height=1.0);
    static void DrawTexture2D(GLuint id,float Brightness=1.0);
    ~Texture2D();
};

#endif // TEXTURE2D_H

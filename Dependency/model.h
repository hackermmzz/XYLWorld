#ifndef MODEL_H
#define MODEL_H
//////////////////////////只能加载obj文件并且只加载顶点数据
#include"tinyObj/tiny_obj_loader.h"
#include"include.h"
#include"vao.h"
#include"vbo.h"
#include"ebo.h"
using namespace tinyobj;
struct Model
{
    Vao*vao;
    Vbo*vbo;
    Ebo*ebo;
    uint VertexCnt;
    ///////////////////////////
    Model(const char*file);
    void Render();
    ~Model();
};

#endif // MODEL_H

#ifndef SECTION_H
#define SECTION_H
#include"globalsource.h"
#include"face.h"
struct Section
{
    bool completePossDataToGpu;
    Chunk*Chunk;//所属的chunk
    vec3 position;//左下角位置
    Vao*vao;
    Vbo*block_render;//存储一个区块中将被渲染的实体方块的面
    vector<Face>face;//将被渲染实体方块的面
    Vao* waterVao;
    Vbo* water_render;
    vector<Face>waterFace;
    vec3 BoxPoint[8];
    Section();
    void PossDataToGpu();
    ~Section();
};

#endif // SECTION_H

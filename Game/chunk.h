#ifndef CHUNK_H
#define CHUNK_H
#include"globalsource.h"
#include"block.h"
#include"other.h"
#include"section.h"
#include"controller.h"
struct Chunk
{
    bool completeLoadBlock;
    uint GenerateFaceState;
    atomic<uint>beUsing;
    vec2 pos;
    BiomeType BiomeId;
    Section* section[SectionBlockWidth];
    int TerrainHeight[SectionBlockWidth][SectionBlockWidth];//高度图
    Block block[SectionBlockWidth][SectionBlockWidth*ChunkSectionWidth][SectionBlockWidth];//存储实体block
    vec3 BoxPoint[8];//包围盒
    /////////////////////////////
    Chunk();
    void Init(const vec2& pos);
    void InitSection();
    ~Chunk();
};
////////////////////////////
#endif // CHUNK_H

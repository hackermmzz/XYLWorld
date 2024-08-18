#include "chunk.h"

Chunk::Chunk():beUsing(0)
{
    for (auto& ele : section)
    {
        ele = new Section();
        ele->Chunk = this;
    }
}

void Chunk::Init(const vec2 &pos_)
{
    beUsing=0;
    completeLoadBlock=0;
    GenerateFaceState=0;
    pos=pos_;
    GenerateBox(BoxPoint,vec3{pos.x,0,pos.y},SectionBlockWidth,SectionBlockWidth,SectionBlockWidth*ChunkSectionWidth);
    InitSection();
}

void Chunk::InitSection()
{
    for(int i=0;i<ChunkSectionWidth;++i){
        section[i]->face.clear();
        section[i]->waterFace.clear();
        section[i]->position=vec3(pos.x,i*SectionBlockWidth,pos.y);
        section[i]->completePossDataToGpu=0;
        GenerateBox(section[i]->BoxPoint,section[i]->position,SectionBlockWidth,SectionBlockWidth,SectionBlockWidth);
    }
}


Chunk::~Chunk()
{
    for(auto&b:section)
     if(b)delete b;
}

Chunk *GetOneChunk()
{
    if(ChunkAllocatePool.size()==0)return new Chunk();
    auto*res=ChunkAllocatePool.back();
    ChunkAllocatePool.pop_back();
    return res;
}

void AppendToChunkAllocatePool(Chunk *chunk)
{
    if(ChunkAllocatePool.size()>=ChunkAllocatePoolMaxSize)delete chunk;
    else ChunkAllocatePool.push_back(chunk);
}

Chunk*GetChunk(const vec2&pos)
{
    static const int LoadSize=WorldChunkWidth+2;
    static const int HalfLoad=LoadSize/2;
    static const int shift=Get_2_N_POW(SectionBlockWidth);
    int xi=(int(pos.x-CameraChunkPos.x)>>shift<<shift)/SectionBlockWidth+HalfLoad;
    int zi=(int(pos.y-CameraChunkPos.y)>>shift<<shift)/SectionBlockWidth+HalfLoad;
    if(xi>=LoadSize||zi>=LoadSize||xi<0||zi<0)return 0;
    return WorldChunk[xi][zi];
}

///////////////////////////////////////////////////////
Block *GetBlock(const glm::vec3 &pos)
{
    static const int ChunkMaxHeight=SectionBlockWidth*ChunkSectionWidth-1;
    static const int mask=SectionBlockWidth-1;
    int yi=pos.y;
    if(yi<0||yi>ChunkMaxHeight)return 0;
    Chunk*chunk=GetChunk(vec2{pos.x,pos.z});
    if(!chunk)return 0;
    return &chunk->block[int(pos.x)&mask][yi][int(pos.z)&mask];
}

Block& GetBlock(int x,int y,int z)
{
    static const int ChunkMaxHeight=SectionBlockWidth*ChunkSectionWidth-1;
    static const int mask=SectionBlockWidth-1;
    static Block defaultBlock{0,0};
    if(y<0||y>ChunkMaxHeight)return defaultBlock;
    Chunk*chunk=GetChunk(vec2{x,z});
    if(!chunk)return defaultBlock;
    return chunk->block[x&mask][y][z&mask];
}

Section*GetSection(const vec3&pos){
    static const int shift=Get_2_N_POW(SectionBlockWidth);
    Chunk*chunk=GetChunk(vec2{pos.x,pos.z});
    if(!chunk)return 0;
    int yi=int(pos.y)>>shift;
    if(yi>=ChunkSectionWidth||yi<0)return 0;
    return chunk->section[yi];
}

void SetBlock(int x, int y, int z, BYTE id, BYTE meta)
{
    Block*block=GetBlock(vec3{x,y,z});
    if(block)
    {
        block->id=id;
        block->meta=meta;
    }
}

bool JudgeBlockCanDestroy(Block&block)
{
        switch (block.id) {
        case BedRock:
        case Water:
            return 0;
        }
        return block.id;
}

bool JudgeBlockIsFunctionBlock(Block&block){
    switch (block.id)
    {
    case TNT:
    case WorkSpace:
        return 1;
    default:
        return 0;
    }
    return 0;
}

bool JudgeIsLeaves(Block&block){
    switch(block.id)
    {
        case CherryLogLeaves:case JungleLogLeaves:case AcaciaLogLeaves:case BirchLogLeaves:
        return 1;
    }
    return 0;
}

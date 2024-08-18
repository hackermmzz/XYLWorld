#include "rolecontrollerglobalsource.h"
FSM<RoleState>*RoleJDFsm;
FSM<RoleState>* RoleMVFsm;
RoleFSM* RoleBuildFsm;
size_t RoleMoveDirection=0;
Cylinder RoleBox = { 0.4,1.8};
void UpdateSectionFaceNearBlock(int x, int y, int z)
{
    auto* chunk = GetChunk(vec2{ x,z });
    int sectionID = y / SectionBlockWidth;
    //本section
    auto* section = chunk->section[sectionID];
    auto cnc = GetChunkNearChunks(chunk);
    GenerateFace(section, cnc);
    section->PossDataToGpu();
    //上
    if ((y+1)%SectionBlockWidth==0 && sectionID + 1 < ChunkSectionWidth)
    {
        auto* section = chunk->section[sectionID + 1];
        GenerateFace(section,cnc);
        section->PossDataToGpu();
    }
    //下
    if ((y % SectionBlockWidth == 0) && sectionID>=1)
    {
    auto* section = chunk->section[sectionID - 1];
    GenerateFace(section, cnc);
    section->PossDataToGpu();
    }
    //左
    if (x % SectionBlockWidth == 0)
    {
        auto* chunk = get<2>(cnc);
        auto* section = chunk->section[sectionID];
        GenerateFace(section, GetChunkNearChunks(chunk));
        section->PossDataToGpu();
    }
    //右
    if ((x+1) %SectionBlockWidth == 0)
    {
        auto* chunk = get<3>(cnc);
        auto* section = chunk->section[sectionID];
        GenerateFace(section, GetChunkNearChunks(chunk));
        section->PossDataToGpu();
    }
    //前
    if ((z+1) %SectionBlockWidth == 0)
    {
        auto* chunk = get<0>(cnc);
        auto* section = chunk->section[sectionID];
        GenerateFace(section, GetChunkNearChunks(chunk));
        section->PossDataToGpu();
    }
    //后
    if (z % SectionBlockWidth == 0)
    {
        auto* chunk = get<1>(cnc);
        auto* section = chunk->section[sectionID];
        GenerateFace(section,GetChunkNearChunks(chunk));
        section->PossDataToGpu();
    }
}

void UpdateBlockChangedMap(const vec3& pos, Block& block)
{
    ////////////////////////////////////
    auto itr=BlockChanged.find(pos);
    if (itr == BlockChanged.end())
    {
        BlockChanged.insert({ pos,block.id });
    }
    else
    {
        itr->second = block.id;
    }
    /////////////////////////////////////如果是发光方块
   /* bool lightable = is_Lightable(block.id);
    if (block.id != EMPTY && !lightable)return;
    auto itr2 = LightBlock.find(pos);
    if (itr2==LightBlock.end()) {
        if (lightable)
            LightBlock.insert({ pos,block });
    }
    else LightBlock.erase(itr2);*/
}

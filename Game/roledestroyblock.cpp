#include "roledestroyblock.h"
void RoleChangeBlock::Enter()
{

}

void RoleChangeBlock::Execute()
{
    static vec3 blockOffset[]{
        {1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
        };
    static int shift = Get_2_N_POW(SectionBlockWidth);
    for (RayMatch ray(controller->camera.position, controller->camera.front); ray.Distance() < BlockTargetMaxDistance; ray.Step(0.1))
    {
        vec3 pos = ray.GetRay();
        pos = vec3{floor(pos.x),floor(pos.y),floor(pos.z)};
        vec3 pos_ = vec3(pos.x, pos.y, pos.z);
        Block& block =GetBlock(pos_.x, pos_.y,pos_.z);
        //////////////////////////////////
        if (block.id == 0 || block.id == Water||!JudgeBlockCanDestroy(block))continue;
        /////////////////////////////////
        BagItems[block.id]++;//放入背包
        ////////////////检测该方块四周是否有水
        if (CheckBlockAroundIsWater(pos_)) {
            vector<vec3>UpdatedsectionPos;
            vec3 sectionPos = { int(pos_.x) >> shift << shift,int(pos_.y) >> shift << shift,int(pos_.z) >> shift << shift };
            UpdatedsectionPos.push_back(sectionPos);
            block.id = Water;
            FillWaterAroundWithWater(pos_,UpdatedsectionPos);
            for (auto& pos : UpdatedsectionPos)
            {
                if (!equal(sectionPos,pos))
                {
                    auto* section = GetSection(pos);
                    GenerateFace(section, GetChunkNearChunks((Chunk*)section->Chunk));
                    section->PossDataToGpu();
                }
            }
        }
        else block.id = 0;
        ////////////////////////////////更新BlockcChanged
        UpdateBlockChangedMap(pos_, block);
        //////////////////////////////算出block所在的section以及其临近的section都将被更新
        UpdateSectionFaceNearBlock(pos_.x,pos_.y,pos_.z);
        break;
    }
    SwitchState(STOP);
}

void RoleChangeBlock::Exit()
{

}

bool CheckBlockAroundIsWater(const vec3& pos_)
{
    if (GetBlock(pos_.x + 1, pos_.y, pos_.z).id == Water ||
        GetBlock(pos_.x - 1, pos_.y, pos_.z).id == Water ||
        GetBlock(pos_.x, pos_.y, pos_.z + 1).id == Water ||
        GetBlock(pos_.x, pos_.y, pos_.z - 1).id == Water
        )
        return 1;
    return 0;
}

void FillWaterAroundWithWater(vec3& pos,vector<vec3>& UpdatedSection)
{
    static int shift = Get_2_N_POW(SectionBlockWidth);
    static vec2 offsets[] = { {1,0},{-1,0},{0,1},{0,-1} };
    for (auto& offset : offsets)
    {
        vec3 pos_ = vec3{ pos.x + offset.x,pos.y,pos.z + offset.y };
        auto& block = GetBlock(pos_.x, pos_.y, pos_.z);
        if (block.id == EMPTY)
        {
            block.id = Water;
            UpdateBlockChangedMap(pos_,block);
            vec3 sectionPos = vec3{int(pos_.x)>>shift<<shift,int(pos_.y)>>shift<<shift,int(pos_.z)>>shift<<shift};
            if (UpdatedSection.end() == find(UpdatedSection.begin(), UpdatedSection.end(),sectionPos))
                UpdatedSection.push_back(sectionPos);
            FillWaterAroundWithWater(pos_,UpdatedSection);
        }
    }
}

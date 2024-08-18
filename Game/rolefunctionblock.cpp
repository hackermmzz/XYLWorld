#include "rolefunctionblock.h"

void RoleFunctionBlock::Enter()
{
}

void RoleFunctionBlock::Execute()
{
    static int MAX_HEIGHT = ChunkSectionWidth * SectionBlockWidth - 1;
    ////////////////////一直射到一个方块为止或者超出射程
    Block* BlockHit = 0;
    vec3 BlockPos;
    auto&camera=controller->camera;
    for (RayMatch ray(camera.position, camera.front); ray.Distance() < BlockTargetMaxDistance; ray.Step(0.1))
    {
        vec3 pos = ray.GetRay();
        pos = vec3{floor(pos.x),floor(pos.y),floor(pos.z)};
        /////////////////////////约束范围
        if (pos.y > MAX_HEIGHT || pos.y < 0)continue;
        //////////////////////////
        vec3 pos_ = vec3(pos.x, pos.y, pos.z);
        Block& block = GetBlock(pos_.x, pos_.y, pos_.z);
        ////////////////////////////////////////
        if (block.id == Water)continue;
        if (block.id == 0)
        {
            if (CheckAroundExistBlock(pos_))
            {
                BlockPos = pos_;
                BlockHit = &block;
            }
        }
        else if(JudgeBlockIsFunctionBlock(block))
        {
            if (DealWithFunctionalBlock(pos_, block)) {
                SwitchState(STOP);
                return;
            }
        }
    }
    /////////////////////////////////////////////
    if (BlockHit) BlockHit_Function(*BlockHit,BlockPos);
    ////////////////////////////////////////////
    SwitchState(STOP);
}

void RoleFunctionBlock::Exit()
{
}

bool CheckAroundExistBlock(const vec3& pos)
{
    static vec3 direction[6] = { {0,0,1},{0,0,-1},{0,1,0},{0,-1,0},{1,0,0},{-1,0,0} };
    for (auto&dir: direction)
    {
        vec3 pos_ = pos + dir;
        auto& block = GetBlock(pos_.x, pos_.y, pos_.z);
        if (block.id&&block.id!=Water)return 1;
    }
    return 0;
}

bool DealWithFunctionalBlock(const vec3&pos,Block& block)
{
    switch (block.id)
    {
    case TNT:
        return TNT_Function(pos);
    case WorkSpace:
        return WORKSPACE_Function();
    }
    return 0;
}

bool TNT_Function(const vec3& pos)
{
    static int shift = Get_2_N_POW(SectionBlockWidth);
    static int MAX_HEIGHT = SectionBlockWidth * ChunkSectionWidth - 1;
    ////////////////////////////////////////
    auto DestroyedBlock = TNT_BOOM(pos);
    set<vec3,VEC3CMP>SectionNeedUpdate;
    ////////////////////////////////////////先获取所有需要更新的section
    for (auto& pos : DestroyedBlock)
    {
        auto sectionPos = vec3(int(pos.x) >> shift << shift, int(pos.y) >> shift << shift, int(pos.z) >> shift << shift);
        SectionNeedUpdate.insert(sectionPos);
        //上
        if (sectionPos.y + SectionBlockWidth < MAX_HEIGHT && int(pos.y) % (SectionBlockWidth - 1) == 0)SectionNeedUpdate.insert(sectionPos+ vec3{ 0,SectionBlockWidth,0 });
        //下
        if (sectionPos.y - SectionBlockWidth >= 0 && int(pos.y) % SectionBlockWidth == 0)SectionNeedUpdate.insert(sectionPos - vec3{ 0,SectionBlockWidth,0 });
        //前
        if (int(pos.z) % (SectionBlockWidth - 1) == 0)SectionNeedUpdate.insert(sectionPos + vec3{ 0,0,SectionBlockWidth });
        //后
        if (int(pos.z) % (SectionBlockWidth) == 0)SectionNeedUpdate.insert(sectionPos - vec3{ 0,0,SectionBlockWidth });
        //左
        if (int(pos.x) % SectionBlockWidth == 0)SectionNeedUpdate.insert(sectionPos - vec3{ SectionBlockWidth,0,0 });
        //右
        if (int(pos.x) % (SectionBlockWidth - 1) == 0)SectionNeedUpdate.insert(sectionPos + vec3{ SectionBlockWidth,0,0 });
    }
    //////////////////////////////////////////////////////
    for (auto& sectionPos : SectionNeedUpdate)
    {
        auto* section = GetSection(sectionPos);
        if (section == 0)continue;
        GenerateFace(section, GetChunkNearChunks((Chunk*)(section->Chunk)));
        section->PossDataToGpu();
    }
    //////////////////////////////////////////
    return 1;
}

vector<vec3> TNT_BOOM(const vec3& pos)
{
    static int DISTANCE_2 = TNTBoomRange * TNTBoomRange;
    static int MAX_HEIGHT = SectionBlockWidth * ChunkSectionWidth - 1;
    ///////////////////////////////////
    vector<vec3>DestroyedBlock;
    //////////////////////////////////////先将自身改成0
    SetBlock(pos.x, pos.y, pos.z, 0, 0);
    //////////////////////////////////////
    for (int x = pos.x - TNTBoomRange; x <= pos.x + TNTBoomRange; ++x)
    {
        for (int y = pos.y - TNTBoomRange; y <= pos.y + TNTBoomRange; ++y)
        {
            for (int z = pos.z - TNTBoomRange; z <= pos.z + TNTBoomRange; ++z)
            {
                int ox, oy, oz;
                ox = x - pos.x, oy = y - pos.y, oz = z - pos.z;
                float distance = ox * ox + oy * oy + oz * oz;
                if (distance > DISTANCE_2 || y > MAX_HEIGHT || y < 0)continue;
                //////////////////////////////////////////
                auto pos = vec3{ x, y, z };
                auto& block = GetBlock(x, y, z);
                //////////////////////如果所炸方块为TNT,则引爆该TNT
                if (block.id == TNT)
                {
                    auto DestroyedBlock_ = TNT_BOOM(pos);
                    for (auto& itr : DestroyedBlock_)DestroyedBlock.push_back(itr);
                    }
                /////////////////////
                else if (JudgeBlockCanDestroy(block)) {
                    //BagItems[block.id]++;
                    block.id = 0;
                }
                UpdateBlockChangedMap(pos, block);
                DestroyedBlock.emplace_back(pos);
                //////////////////////////////////////////
            }
        }
    }
    //////////////////////////////////////////////////
    return DestroyedBlock;
}

void BlockHit_Function(Block& block, const vec3& pos)
{
    ///////////////////////
    auto id =ItemBoardAllItems[ItemBoardCurrentSelectedItem]->id;
    if (BagItems[id]==0)return;
    ///////////////////////检测所放位置是否已经和人物位置冲突
    auto&camera=controller->camera;
    vec3 center = vec3(camera.position.x, camera.position.y - RoleBox.h/2, camera.position.z);
    AABB roleBoxAABB(center, vec3(RoleBox.r * 2), 1);
    AABB blockAABB(vec3(pos.x, pos.y, pos.z) + vec3(0.5, 0.5, 0.5), vec3(1), 1);
    if (roleBoxAABB.IsCollide(blockAABB))return;
    ////////////////////////////
    BagItems[id]--;
    block.id = id;
    SetBlockFaceDir(block);//设置面朝向
    //////////////////////
    UpdateBlockChangedMap(pos, block);
    //////////////////////
    UpdateSectionFaceNearBlock(pos.x,pos.y,pos.z);
}

void SetBlockFaceDir(Block& block)
{
    if (JudgeIsPlant(block))return;
    ////////////////////////////////////
    auto& dir = controller->camera.front;
    if (abs(dir.x) > abs(dir.z))
    {
        block.meta = dir.x < 0 ? 5 : 4;
    }
    else
    {
        block.meta = dir.z < 0 ? 2 : 3;
    }
}

bool WORKSPACE_Function()
{
    ReleaseMouse();
    ///////////////////重置工作台
    auto* Bag = workSpaceBoard->ui->Bag;
    for (int i = 0; i < ITEM_NUMS; ++i)
    {
        int r = i / BagColumn, c = i % BagColumn;
        auto* widget = Bag->cellWidget(r, c);
        auto* num = widget->findChild<QLabel*>("number");
        num->setText(to_string(BagItems[i]).c_str());
    }
    for (auto& source : WorkSpaceBoardSource)
    {
        source->setIcon(ItemIcons[0], 0);
    }
    workSpaceBoard->ui->target->setIcon(ItemIcons[0], 0);
    ///////////////////
    workSpaceBoard->show();
    //////////////////
    return 1;
}

void UpdateBagItem(int id, int nums)
{

}

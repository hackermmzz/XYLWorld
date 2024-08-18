#ifndef ROLEFUNCTIONBLOCK_H
#define ROLEFUNCTIONBLOCK_H
#include"rolecontrollerglobalsource.h"
#include"qmessagebox.h"
struct RoleFunctionBlock:RoleAction
{
    // 通过 FSMAction 继承
    void Enter() override;
    void Execute() override;
    void Exit() override;
};
bool CheckAroundExistBlock(const vec3& pos);
bool DealWithFunctionalBlock(const vec3&pos,Block& block);
bool TNT_Function(const vec3&pos);
bool WORKSPACE_Function();
void UpdateBagItem(int id,int nums);
vector<vec3> TNT_BOOM(const vec3& pos);//返回会被更新的section坐标
void BlockHit_Function(Block&block,const vec3&pos);
void SetBlockFaceDir(Block& block);

#endif // ROLEFUNCTIONBLOCK_H

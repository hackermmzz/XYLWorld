#ifndef ROLEDESTROYBLOCK_H
#define ROLEDESTROYBLOCK_H
#include"rolecontrollerglobalsource.h"
struct RoleChangeBlock:RoleAction
{
    void Enter();
    void Execute();
    void Exit();
};
bool CheckBlockAroundIsWater(const vec3& pos);
void FillWaterAroundWithWater(vec3& pos,vector<vec3>&UpdatedSection);
#endif // ROLEDESTROYBLOCK_H

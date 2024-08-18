#ifndef ROLECONTROLLERGLOBALSOURCE_H
#define ROLECONTROLLERGLOBALSOURCE_H
#include"globalsource.h"
#include"generateterrain.h"
#include"generateface.h"
struct Cylinder
{
    float r;
    float h;
};
enum RoleState{
    STOP,
    MOVE,
    JUMP,
    DROP,
    DESTROY_BLOCK,
    FUNCTION_BLOCK,
};
using RoleAction=FSMAction<RoleState>;
void UpdateSectionFaceNearBlock(int x, int y, int z);///////////////////////更新一个block的六个临近section
void UpdateBlockChangedMap(const vec3& pos, Block& block);
using RoleFSM=FSM<RoleState>;
extern size_t RoleMoveDirection;
extern Cylinder RoleBox;//人物圆柱体包围盒
#endif // ROLECONTROLLERGLOBALSOURCE_H

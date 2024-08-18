#include "rolestop.h"

RoleStop::RoleStop(bool controlDrop_)
:controlDrop(controlDrop_)
{

}

void RoleStop::Enter()
{

}

void RoleStop::Execute()
{
    if (!controlDrop)return;
    static vec2 offsets[] = {{-RoleBox.r,-RoleBox.r},{-RoleBox.r,0},{0,-RoleBox.r},{0,0} };
    auto&camera=controller->camera;
    int y = floor(camera.position.y - RoleBox.h-0.01);
    vec2 camera_pos = vec2(camera.position.x, camera.position.z);
    for (auto& offset : offsets)
    {
        vec2 temp= (offset +camera_pos);
        vec2 block_pos =vec2{floor(temp.x),floor(temp.y)};
        if (JudgeBlockCanBarry(GetBlock(block_pos.x, y, block_pos.y)))return;
    }
    SwitchState(DROP);
}

void RoleStop::Exit()
{

}


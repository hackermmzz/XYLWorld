#include "roledrop.h"
void RoleDrop::Enter()
{
    last_time=QDateTime::currentMSecsSinceEpoch();
    speed=0;
}

void RoleDrop::Execute()
{

    static float minus = std::numeric_limits<float>::epsilon();
    auto time=QDateTime::currentMSecsSinceEpoch();
    auto delta_time=(time-last_time)/1000.0;
    last_time=time;
    /////////////////////////////掉落
    float delta_y =speed * delta_time;
    speed+=delta_time*10.0;
    speed = fmin(speed, 20);
    /////////////////////////////
    vec3&CP=controller->camera.position;
    vec3 block_pos =vec3{floor(CP.x),floor(CP.y),floor(CP.z)};
    int y = floor(CP.y - RoleBox.h);
    int y_end = floor(CP.y - RoleBox.h - delta_y);
    for (; y>= y_end; --y)
    {
        if (JudgeBlockCanBarry(GetBlock(block_pos.x, y, block_pos.z)))
        {
            CP.y=y+1+RoleBox.h+minus;
            SwitchState(STOP);
            return;
        }
    }
    CP.y -= delta_y;
}

void RoleDrop::Exit()
{

}

bool RoleDrop::AllowSwitchState(const RoleState& state)
{
    if (state == JUMP)return 0;
    return 1;
}

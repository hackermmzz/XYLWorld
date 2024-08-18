#ifndef CONTROLLE_H
#define CONTROLLE_H
#include"globalsource.h"
#include"role.h"
struct Controller
{
    Role role;
    Camera camera;
    Ubo*ubo;//包括model view perspective model*view*perspective
    /////////////////////////////
    Controller(const vec3&InitPos);
    void updateCamera();
    void setPos(const vec3&pos);
};

#endif // CONTROLLE_H

#ifndef ROLE_H
#define ROLE_H
#include"globalsource.h"
struct Role
{
    vec3 pos;
    mat4 model;
    ///////////////////////////
    Role(const vec3&initPos,const mat4&model);
};

#endif // ROLE_H

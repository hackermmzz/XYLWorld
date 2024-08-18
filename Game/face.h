#ifndef FACE_H
#define FACE_H
#include"globalsource.h"
struct FaceVertex
{
    vec3 position;
    vec2 texcoord;
};
struct Face
{
    FaceVertex vertex[4];
};

#endif // FACE_H

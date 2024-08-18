#ifndef AABB_H
#define AABB_H
#include"include.h"
////////////////////////////////////
struct Ray {
    vec3 origin;
    vec3 direction;
    Ray(const vec3& origin, const vec3& direction);
    Ray();
};
//////////////////////////////////////////////////
class RayMatch
{
private:
    Ray ray;
    vec3 CurrentPos;
public:
    RayMatch(const vec3&origin,const vec3&direction);
    const vec3 &GetRay();
    void Step(float scale);
    float Distance();
    ~RayMatch();
};
////////////////////////////////////
class AABB {
private:
    vec3 minVec, maxVec;
public:
    AABB(const vec3& center, const vec3& size, bool reserved);
    AABB(const vec3& minVec,const vec3& maxVec);
    bool IsCollide(const AABB&box);
    bool IsIntersect(const Ray&ray);
    bool IsContainAABB(const AABB& box);
    vec3& GetMinVec();
    vec3& GetMaxVec();
};
#endif // AABB_H

#ifndef COLLITION_H
#define COLLITION_H

#include "math.h"

// Collision Detection TEST...
struct AABB
{
    Vec3 c;       // center point of AABB
    float r[3];   // radius or halfwidth extends (rx, ry, rz)
};


int TestAABBAABB(AABB a, AABB b);
int XZRayIntersectAABB(Vec3 v, Vec3 d, AABB target, Vec3& contactPoint, Vec3& contactNormal, float& tHitNear);
#endif

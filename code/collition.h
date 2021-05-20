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

#endif

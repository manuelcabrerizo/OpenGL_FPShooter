#include "collition.h"
#include "utility.h"

int TestAABBAABB(AABB a, AABB b)
{
    if(absf(a.c.x - b.c.x) > (a.r[0] + b.r[0])) return 0;
    if(absf(a.c.y - b.c.y) > (a.r[1] + b.r[1])) return 0;
    if(absf(a.c.z - b.c.z) > (a.r[2] + b.r[2])) return 0;
    return 1;
}


int TestOverAABBAABB(AABB a, AABB b)
{
    if(absf(a.c.x - b.c.x) > (a.r[0] + b.r[0])) return 0;
    if(absf(a.c.z - b.c.z) > (a.r[2] + b.r[2])) return 0;
    return 1; 
}

int XZRayIntersectAABB(Vec3 v, Vec3 d, AABB target, Vec3& contactPoint, Vec3& contactNormal, float& tHitNear)
{
    float targetX = (target.c.x - target.r[0]) - 0.1f;
    float targetZ = (target.c.z - target.r[2]) - 0.1f;
    float targetWidth = (target.r[0] * 2.0f) + 0.2f;
    float targetDepth = (target.r[2] * 2.0f) + 0.2f;

    Vec3 targetPos  = {targetX, 0.0f, targetZ};
    Vec3 targetSize = {targetWidth, 0.0f, targetDepth};

    Vec3 tNear = (targetPos - v) / d;
    Vec3 tFar  = (targetPos + targetSize - v) / d;

    if(tNear.x > tFar.x)
        SwapFloat(tNear.x, tFar.x);
    if(tNear.z > tFar.z)
        SwapFloat(tNear.z, tFar.z);
    if(tNear.x > tFar.z || tNear.z > tFar.x)
        return 0;

    tHitNear = MaxFloat(tNear.x, tNear.z);
    float tHitFar = MinFloat(tFar.x, tFar.z);
    if(tHitFar < 0)
        return 0;

    Vec3 result = d * tHitNear;
    contactPoint = v + result;

    if(tNear.x > tNear.z)
    {
        if(d.x < 0)
            contactNormal = {1.0f, 0.0f, 0.0f};
        else
            contactNormal = {-1.0f, 0.0f, 0.0f};
    }
    else if(tNear.x < tNear.z)
    {
        if(d.z < 0)
            contactNormal = {0.0f, 0.0f, 1.0f};
        else
            contactNormal = {0.0f, 0.0f, -1.0f};
    }
    return 1;
}

int XZRayIntersectAABBX(Vec3 v, Vec3 d, AABB target, Vec3& contactPoint, Vec3& contactNormal, float& tHitNear)
{
    float targetX = (target.c.x - target.r[0]);
    float targetZ = (target.c.z - target.r[2]);
    float targetWidth = (target.r[0] * 2.0f);
    float targetDepth = (target.r[2] * 2.0f);

    Vec3 targetPos  = {targetX, 0.0f, targetZ};
    Vec3 targetSize = {targetWidth, 0.0f, targetDepth};

    Vec3 tNear = (targetPos - v) / d;
    Vec3 tFar  = (targetPos + targetSize - v) / d;

    if(tNear.x > tFar.x)
        SwapFloat(tNear.x, tFar.x);
    if(tNear.z > tFar.z)
        SwapFloat(tNear.z, tFar.z);
    if(tNear.x > tFar.z || tNear.z > tFar.x)
        return 0;

    tHitNear = MaxFloat(tNear.x, tNear.z);
    float tHitFar = MinFloat(tFar.x, tFar.z);
    if(tHitFar < 0)
        return 0;

    Vec3 result = d * tHitNear;
    contactPoint = v + result;

    if(tNear.x > tNear.z)
    {
        if(d.x < 0)
            contactNormal = {1.0f, 0.0f, 0.0f};
        else
            contactNormal = {-1.0f, 0.0f, 0.0f};
    }
    else if(tNear.x < tNear.z)
    {
        if(d.z < 0)
            contactNormal = {0.0f, 0.0f, 1.0f};
        else
            contactNormal = {0.0f, 0.0f, -1.0f};
    }
    return 1;
}


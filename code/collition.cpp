#include "collition.h"

int TestAABBAABB(AABB a, AABB b)
{
    if(absf(a.c.x - b.c.x) > (a.r[0] + b.r[0])) return 0;
    if(absf(a.c.y - b.c.y) > (a.r[1] + b.r[1])) return 0;
    if(absf(a.c.z - b.c.z) > (a.r[2] + b.r[2])) return 0;
    return 1;
}

/* Returns indices imin and imax into pt[] array of the least and
// most, respectively, distant points along the direction dir
void ExtremePointsAlongDirection(Vector dir, Point pt[], int n, int *imin, int *imax)
{
    float minproj = FLT_MAX, maxproj = -FLT_MAX;
    for (int i = 0; i < n; i++)
    {
        // Project vector from origin to point onto direction vector
        float proj = Dot(pt[i], dir);
        // Keep track of least distant point along direction vector
        if (proj < minproj) 
        {
            minproj = proj;
            *imin = i;
        }
        // Keep track of most distant point along direction vector
        if (proj > maxproj)
        {
            maxproj = proj;
            *imax = i;
        }
    }
}
*/

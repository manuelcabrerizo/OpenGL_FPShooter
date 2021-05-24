#ifndef ENTITIES_H
#define ENTITIES_H

#include "collition.h"
#include "camera.h"

struct Projectile
{
    Vec3 position;
    Vec3 start;
    Vec3 end;
    float speed;
    float distance;
    bool impactSomething;
};

struct Enemy
{
    AABB collider;
    Vec3 position; 
    int life;
    float rotation;
};

struct Building
{
    AABB collider;
    Vec3 position;
    Vec3 scale;
};

void ProcessPlayerMovement(Input* input, Camera* camera, Building* buildings, float deltaTime);
void ProcessEnemyMovementAndCollition(Enemy* enemies,
                                      int numEnemies,
                                      Building* buildings,
                                      int numBuildings,
                                      Camera* camera,
                                      float deltaTime);
void ShootProjectile(Projectile* projectile, Vec3 start, Vec3 end);
Matrix UpdateProjectile(Projectile* projectile, float deltaTime);

#endif

#ifndef ENTITIES_H
#define ENTITIES_H

#include "collition.h"
#include "camera.h"

#define GRAVITY 0.001
#define JUMP_POWER 0.06

struct Transform
{
    Vec3 position;
    Vec3 scale;
    AABB collider;
};

struct Projectile
{
    Vec3 position;
    Vec3 start;
    Vec3 end;
    float speed;
    float distance;
    bool impactSomething;
    Matrix model;
};

struct Enemy
{
    AABB collider;
    Vec3 position; 
    int life;
    float rotation;
    Matrix model;
};

struct Building
{
    AABB collider;
    Vec3 position;
    Vec3 scale;
    Matrix model;
};

struct Weapon
{
    Vec3 position;
    Projectile projectile[200];
    AABB     projCollider[200];
    Matrix model;
};

float GetEntityHeight(float xPos,
                      float yPos,
                      float mapHeigt[],
                      int numCols,
                      int numRows);
void ProcessEnemyMovementAndCollition(Enemy* enemies,
                                      int numEnemies,
                                      Building* buildings,
                                      int numBuildings,
                                      Camera* camera,
                                      float deltaTime);
void ShootProjectile(Projectile* projectile, Vec3 start, Vec3 end);
Matrix UpdateProjectile(Projectile* projectile, float deltaTime);

void ProcessPlayerJump(float playerHeight,
                       float scale,
                       Camera* camera);

void ProcessPlayerMovement(Input* input,
                           Camera* camera,
                           Building* buildings,
                           float deltaTime,
                           float mapHeigt[],
                           AABB collider);

#endif

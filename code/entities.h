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

struct Weapon
{
    unsigned int wVao;
    unsigned int pVao;
    unsigned int wTexId;
    unsigned int pTexId;
    Projectile projectile[200];
    AABB     projCollider[200];
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

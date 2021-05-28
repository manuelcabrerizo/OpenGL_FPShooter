#ifndef ENTITIES_H
#define ENTITIES_H

#include "collition.h"
#include "camera.h"
#include "utility.h"

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
    bool shouldRender;
};

struct Enemy
{
    AABB collider;
    Vec3 position; 
    int life;
    float rotation;
    Matrix model;
    bool shouldRender;
};

struct Building
{
    AABB collider;
    Vec3 position;
    Vec3 scale;
    Matrix model;
    bool shouldRender;
};

struct Weapon
{
    Vec3 position;
    Projectile projectile[200];
    AABB projCollider[200];
    Matrix* projectileModels[200];
    bool* projectileShouldRender[200];
    Matrix model;
    bool shouldRender;
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

void InitializeWeapon(Weapon* weapon,
                      Mesh* projectileMesh,
                      Shader* shader,
                      int numberBullets);

void InitializeBuildings(Building* buildings,
                         Cube* buildingsMesh,
                         int numberBuildings,
                         Shader* shader,
                         Matrix* buildingsModels[],
                         bool* buildingsShouldRender[]);

void InitializeEnemies(Enemy* enemy,
                       Mesh* mesh,
                       int numberEnemies,
                       Shader* shader,
                       Matrix* enemiesModels[],
                       bool* enemyShouldRender[]);

#endif

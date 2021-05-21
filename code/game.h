#ifndef GAME_H
#define GAME_H

#include "shader.h"
#include "utility.h"
#include "input.h"
#include "camera.h"
#include "line.h"
#include "collition.h"

#define global_variable static
#define WNDWIDTH 800
#define WNDHEIGHT 600
#define LEFTBUTTON 0
#define RIGHTBUTTON 1

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
};

struct Building
{
    AABB collider;
    Vec3 position;
    Vec3 scale;
};

struct UI
{
    Line xAxis;
    Line yAxis;
    Shader shader;
};

struct MainGame
{
    Shader main_shader;
    Shader mesh_shader; 
    UI ui; 
    Input input;
    Camera camera;
    Terrain terrain;
    Line xAxis;
    Line yAxis;
    Line zAxis;
   
    Mesh colliderMesh;

    Mesh naruto;
    Enemy enemy[5];

    Mesh pistol;
    Mesh ball;
    Projectile projectile[200];
    AABB     projCollider[200];

    Building buildings[4]; 
};

void GameInit(MainGame* game);
void GameUnpdateAndRender(MainGame* game, float deltaTime);

#endif

#ifndef GAME_H
#define GAME_H

#include "shader.h"
#include "utility.h"
#include "input.h"
#include "line.h"
#include "entities.h"

#define global_variable static
#define WNDWIDTH 800
#define WNDHEIGHT 600
#define LEFTBUTTON 0
#define RIGHTBUTTON 1



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
    Shader skybox_shader; 
    UI ui; 
    Input input;
    Camera camera;
    Terrain terrain;
    SkyBox skyBox;
    Line xAxis;
    Line yAxis;
    Line zAxis;
   
    Mesh colliderMesh;

    Mesh naruto;
    Enemy enemy[49];

    Mesh pistol;
    Mesh ball;
    Projectile projectile[200];
    AABB     projCollider[200];

    Building buildings[4]; 
};

void GameInit(MainGame* game);
void GameUnpdateAndRender(MainGame* game, float deltaTime);

#endif

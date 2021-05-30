#ifndef GAME_H
#define GAME_H

#include "shader.h"
#include "utility.h"
#include "input.h"
#include "line.h"
#include "player.h"

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
    
    Terrain terrain;
    Matrix* terrainModels[1];
    bool* terrainShouldRender[1];

    float mapHeigt[50*50];
    SkyBox skyBox;
    
    // new Player test...
    Player player;
   
    Mesh colliderMesh;
    Cube colliderCube;

    Mesh naruto;
    Enemy enemy[49];
    Matrix* enemiesModels[49];
    bool* enemyShouldRender[49];

    Mesh pistol;
    Mesh ball;

    bool* buildingsShouldRender[4];
    Matrix* buildingsModels[4];
    Building buildings[4];


    unsigned int colladaVao;
    unsigned int colladaTexId;
    int colladaMeshNumVertex;

    Mesh cowboy;
    

};

void GameInit(MainGame* game);
void GameUnpdateAndRender(MainGame* game, float deltaTime);

#endif

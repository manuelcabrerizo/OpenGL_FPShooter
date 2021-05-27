#ifndef PLAYER_H
#define PLAYER_H

#include "entities.h"

struct Player
{
    unsigned int vao;
    unsigned int textId;
    Transform transform;
    Vec3 velocity; 
    Camera camera;
    Weapon weapon;
    bool isJumping;
    float jumpPower;
    bool shouldRender;
};


void ProcessPlayerJumpTest(Player* player,
                           float playerHeight);

void ProcessPlayerMovementTest(Player* player,
                               Input* input,
                               float deltaTime);

void ProcessPlayerCollitions(Player* player,
                             Building* buildings,
                             float mapHeigt[],
                             float deltaTime);

Matrix ProcessPlayerWeapon(Weapon* weapon,
                           Camera* camera,
                           Input* input,
                           float deltaTime);

void ProcessPlayerProjectiles(Player* player,
                              Building* buildings,
                              Enemy* enemy, 
                              float deltaTime);

#endif


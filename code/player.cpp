
#include "player.h"

void ProcessPlayerJumpTest(Player* player, float playerHeight)
{
    if(player->isJumping)
    {
        player->jumpPower -= GRAVITY;
        player->transform.position.y += player->jumpPower;
        if(player->transform.position.y < playerHeight)
        {
            player->transform.position.y = playerHeight;
            player->isJumping = false;
        }
    }
    else
    {
        player->transform.position.y = playerHeight;
    }
}

void ProcessPlayerMovementTest(Player* player,
                               Input* input,
                               float deltaTime)
{
    Camera* camera = &player->camera;
    player->velocity = {0.0f, 0.0f, 0.0f};
    if(GetKeyDown(input, 'W'))
    {
        player->velocity +=  normaliza_vec3(camera->front);
        camera->isMoving = true;
    }
    if(GetKeyDown(input, 'S'))
    {
        player->velocity +=  -normaliza_vec3(camera->front);
        camera->isMoving = true;
    }
    if(GetKeyDown(input, 'A'))
    {
        player->velocity += normaliza_vec3(camera->right);
        camera->isMoving = true;
    }
    if(GetKeyDown(input, 'D'))
    {
        player->velocity += -normaliza_vec3(camera->right);
        camera->isMoving = true;
    }
    if(!GetKeyDown(input, 'W') && !GetKeyDown(input, 'S') && !GetKeyDown(input, 'A') && !GetKeyDown(input, 'D'))
    {
        camera->isMoving = false;
    }
    if(GetKeyDown(input, 0x20) && player->isJumping == false)
    {
        player->isJumping = true;
        player->jumpPower = JUMP_POWER;
    }
    if(vec3_length(player->velocity) != 0.0f)
    { 
        player->velocity = normaliza_vec3(player->velocity);
    }
}

void ProcessPlayerCollitions(Player* player,
                             Building* buildings,
                             float mapHeigt[],
                             float deltaTime)
{
    float t = 0;
    Vec3 hitPoint  = {0.0f, 0.0f, 0.0f};
    Vec3 hitNormal = {0.0f, 0.0f, 0.0f};
    bool isGrouded = true;
    float playerOverBoxHeight = 0.0f;
    for(int i = 0; i < 4; i++)
    {
        if(XZRayIntersectAABBX(player->transform.position, player->velocity, buildings[i].collider, hitPoint, hitNormal, t) && t <= 1.0f)
        {
            if(TestAABBAABB(player->transform.collider, buildings[i].collider) == 1)
            {
                Vec3 temp = {absf(player->velocity.x), player->velocity.y, absf(player->velocity.z)};
                if((temp.x == 0 && temp.y == 0 && temp.z == 0) ||
                    (hitNormal.x == 0 && hitNormal.y == 0 && hitNormal.z == 0))
                {
                    player->velocity = {0.0f, 0.0f, 0.0f};
                }
                else
                {
                    player->velocity += hitNormal * temp * (1.0f - t);
                }
            }
        }
        if(PointOverAABB(player->transform.collider.c, buildings[i].collider) == 1 &&
           TestAABBAABB(player->transform.collider, buildings[i].collider) == 0 &&
           player->transform.collider.c.y - buildings[i].collider.c.y > 0.0f)
        {
            playerOverBoxHeight = (buildings[i].collider.c.y + buildings[i].collider.r[1]) + 1.1f;
            isGrouded = false;
        }
    } 
    float playerHeight = 0.0f;
    if(isGrouded) playerHeight = GetEntityHeight(player->transform.position.x, player->transform.position.z, mapHeigt, 50, 50) + 1.1f;
    else playerHeight = playerOverBoxHeight;   
    if(player->transform.position.y > playerHeight) player->isJumping = true; 
    ProcessPlayerJumpTest(player, playerHeight);

    player->transform.position += (player->velocity * 2.0f) * deltaTime;
    player->transform.collider.c = player->transform.position;
    player->camera.position = player->transform.position;
    player->camera.viewMat = get_view_matrix(player->camera.position,
                                      player->camera.position + player->camera.target,
                                      player->camera.up);
}


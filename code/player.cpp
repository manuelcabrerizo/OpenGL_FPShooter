
#include "player.h"

#include <math.h>

#define LEFTBUTTON 0
#define RIGHTBUTTON 1

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

Matrix ProcessPlayerWeapon(Weapon* weapon,
                           Camera* camera,
                           Input* input,
                           float deltaTime)
{
    Vec3 zOffset = normaliza_vec3(camera->target);
    Vec3 xOffset = normaliza_vec3(camera->right);
    Vec3 pistolFinalPos = camera->position + zOffset*0.2f + xOffset*0.05f; 
    if(camera->isMoving == true)
    {  
        static float time = 0.0f;
        pistolFinalPos.y = pistolFinalPos.y + sinf(time) / 50.0f;
        time += deltaTime * 15.0f;
    } 
    weapon->position = pistolFinalPos;
    Matrix pistolPos = get_translation_matrix(pistolFinalPos);
    Matrix pistolRot = get_rotation_y_matrix(-to_radiant(camera->yaw + -90.0f)) *
                       get_rotation_arbitrary_matrix(xOffset, to_radiant(camera->pitch));
    Matrix pistolModel = get_scale_matrix({0.01f, 0.01f, 0.01f}) * pistolRot * pistolPos;

    // START::PROCCESING::OUR::PROJECTILE::STUFF...
    static bool mouseLButtonPress = false;
    static int actualProjectile   = 0;
    if((GetMouseButtonPress(input, LEFTBUTTON) || input->A == true) && mouseLButtonPress == false)
    { 
        Vec3 bulletPos = weapon->position;
        bulletPos.y -= 0.03f;
        ShootProjectile(&weapon->projectile[actualProjectile], bulletPos, camera->position + (normaliza_vec3(camera->target) * 20.0f));
        mouseLButtonPress = true;
        actualProjectile++;
    }
    if((!GetMouseButtonPress(input, LEFTBUTTON) && input->A == false) && mouseLButtonPress == true)
    {
        mouseLButtonPress = false;
    }
    actualProjectile %= 200;

    return pistolModel;
}


void ProcessPlayerProjectiles(Player* player,
                              Building* buildings,
                              Enemy* enemy, 
                              float deltaTime)
{
    for(int i = 0 ; i < 200; i++)
    {
        Projectile* actualProjectile = &player->weapon.projectile[i];
        AABB* actualProjCollider = &player->weapon.projCollider[i];

        actualProjectile->model = get_scale_matrix({0.05f, 0.05f, 0.05f}) * UpdateProjectile(actualProjectile, deltaTime);
        actualProjCollider->c = actualProjectile->position;
        if(actualProjectile->distance <= 1.0f && actualProjectile->impactSomething == false)
        {
            float t = 0;
            Vec3 hitPoint  = {0.0f, 0.0f, 0.0f};
            Vec3 hitNormal = {0.0f, 0.0f, 0.0f}; 
            for(int j = 0; j < 4; j++)
            {
                Vec3 direction = actualProjectile->end - actualProjectile->start;
                if(XZRayIntersectAABBX(actualProjectile->start, direction, buildings[j].collider, hitPoint, hitNormal, t) == 1 && t >= 0.0f && t <= 1.0f)
                { 
                    if(actualProjectile->distance > t)
                    {
                        if(TestAABBAABB(*actualProjCollider, buildings[j].collider) == 1 && actualProjectile->impactSomething == false)
                        {
                            Vec3 newTarget = normaliza_vec3(Vec3Reflect(actualProjectile->start, actualProjectile->end, hitNormal));
                            ShootProjectile(actualProjectile, actualProjectile->position, actualProjectile->position + (newTarget * 20.0f));
                        }
                    }               
                }          
            }
            for(int j = 0; j < 49; j++)
            {
                if(enemy[j].life > 0)
                {
                    if(TestAABBAABB(*actualProjCollider, enemy[j].collider) == 1 && actualProjectile->impactSomething == false)
                    {
                        enemy[j].life--;
                        actualProjectile->impactSomething = true;
                    }
                }
            }
        }

        if(actualProjectile->impactSomething || actualProjectile->distance > 1.0f)
        {
            actualProjectile->shouldRender = false;
        }
    }

}


#include "entities.h"
#include <math.h>

void ProcessPlayerMovement(Input* input, Camera* camera, Building* buildings, float deltaTime)
{
    Vec3 playerVelocity = {0.0f, 0.0f, 0.0f};
    if(GetKeyDown(input, 'W'))
    {
        playerVelocity +=  normaliza_vec3(camera->front);
        camera->isMoving = true;
    }
    if(GetKeyDown(input, 'S'))
    {
        playerVelocity +=  -normaliza_vec3(camera->front);
        camera->isMoving = true;
    }
    if(GetKeyDown(input, 'A'))
    {
        playerVelocity += normaliza_vec3(camera->right);
        camera->isMoving = true;
    }
    if(GetKeyDown(input, 'D'))
    {
        playerVelocity += -normaliza_vec3(camera->right);
        camera->isMoving = true;
    }
    if(!GetKeyDown(input, 'W') && !GetKeyDown(input, 'S') && !GetKeyDown(input, 'A') && !GetKeyDown(input, 'D'))
    {
        camera->isMoving = false;
    }
    Vec3 normPlayerVelocity = {0.0f, 0.0f, 0.0f};
    if(vec3_length(playerVelocity) != 0.0f)
    { 
        normPlayerVelocity = normaliza_vec3(playerVelocity);
    }

    float t = 0;
    Vec3 hitPoint  = {0.0f, 0.0f, 0.0f};
    Vec3 hitNormal = {0.0f, 0.0f, 0.0f}; 
    for(int i = 0; i < 4; i++)
    {
        if(XZRayIntersectAABB(camera->position, normPlayerVelocity, buildings[i].collider, hitPoint, hitNormal, t) && t <= 1.0f)
        {
            Vec3 temp = {absf(normPlayerVelocity.x), normPlayerVelocity.y, absf(normPlayerVelocity.z)};
            normPlayerVelocity += hitNormal * temp * (1.0f - t);
        }
    }
    camera->position += (normPlayerVelocity * 2.0f) * deltaTime; 
    camera->viewMat = get_view_matrix(camera->position, camera->position + camera->target, camera->up);  
}


void ProcessEnemyMovementAndCollition(Enemy* enemies,
                                      int numEnemies,
                                      Building* buildings,
                                      int numBuildings,
                                      Camera* camera, 
                                      float deltaTime)
{
    for(int i = 0; i < numEnemies; i++)
    {
        Enemy* actualEnemy = &enemies[i];
        if(actualEnemy->life > 0)
        {
            Vec3 temp = {1.0f, 0.0f, 1.0f};
            Vec3 enemyVelocity = normaliza_vec3((camera->position - actualEnemy->position) * temp);
        
            float t = 0;
            Vec3 hitPoint  = {0.0f, 0.0f, 0.0f};
            Vec3 hitNormal = {0.0f, 0.0f, 0.0f};

            for(int j = 0; j < numEnemies; j++)
            {
                if(i != j && enemies[j].life > 0)
                {
                    if(XZRayIntersectAABB(actualEnemy->collider.c, enemyVelocity, enemies[j].collider, hitPoint, hitNormal, t) && t <= 1.0f)
                    {
                        Vec3 tempVel = {absf(enemyVelocity.x), enemyVelocity.y, absf(enemyVelocity.z)};
                        enemyVelocity += hitNormal * tempVel * (1.0f - t); 
                    }
                }
            }
            for(int j = 0; j < numBuildings; j++)
            {
                if(XZRayIntersectAABB(actualEnemy->collider.c, enemyVelocity, buildings[j].collider, hitPoint, hitNormal, t) && t <= 1.0f)
                {
                    Vec3 tempVel = {absf(enemyVelocity.x), enemyVelocity.y, absf(enemyVelocity.z)};
                    enemyVelocity += hitNormal * tempVel * (1.0f - t); 
                }
            }

            if(vec3_length(actualEnemy->position - camera->position) < 10.0f &&
               vec3_length(actualEnemy->position - camera->position) > 1.5f)
            {  
                Vec3 enemyDefaultDir = {0.0f, 0.0f, 1.0f};
                actualEnemy->rotation = acosf(vec3_dot(normaliza_vec3(enemyDefaultDir), normaliza_vec3(enemyVelocity)));
                if(camera->position.x <= actualEnemy->position.x)
                {
                    actualEnemy->rotation = to_radiant(360.0f) - actualEnemy->rotation;
                }
                actualEnemy->position   += (enemyVelocity * 1.0f) * deltaTime;
                actualEnemy->collider.c += (enemyVelocity * 1.0f) * deltaTime;
            }

        }
     }
}

void ShootProjectile(Projectile* projectile, Vec3 start, Vec3 end)
{
    projectile->start = start;
    projectile->end   = end;
    projectile->distance = 0.0f;
    projectile->impactSomething = false;
}

Matrix UpdateProjectile(Projectile* projectile, float deltaTime)
{
    Matrix model = get_identity_matrix();
    if(projectile->distance <= 1.0f)
    {        
        projectile->position = Lerp(projectile->start, projectile->end, projectile->distance); 
    } 
    model = get_translation_matrix(projectile->position);
    projectile->distance += projectile->speed * deltaTime;
    return model;
}

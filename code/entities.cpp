#include "entities.h"
#include <math.h>

#include "utility.h"

#include <windows.h>
#include <stdio.h>

#define GRAVITY 0.001
#define JUMP_POWER 0.06

float GetEntityHeight(float xPos, float yPos, float mapHeigt[], int numCols, int numRows)
{ 
    xPos += 25;
    yPos += 25; 
    float x = floorf(xPos);
    float y = floorf(yPos);
    float dx = xPos - x;
    float dz = yPos - y; 
	float A = GetHeightFromHeightMap(mapHeigt, numCols, numRows, x, y);
	float B = GetHeightFromHeightMap(mapHeigt, numCols, numRows, x + 1, y);
	float C = GetHeightFromHeightMap(mapHeigt, numCols, numRows, x, y + 1);
	float D = GetHeightFromHeightMap(mapHeigt, numCols, numRows, x + 1, y + 1);
	float height = 0.0f;
	if(dz < 1.0f - dx)  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C
		height = A + LerpFloat(0.0f, uy, dx) + LerpFloat(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
		height = D + LerpFloat(0.0f, uy, 1.0f - dx) + LerpFloat(0.0f, vy, 1.0f - dz);
	}
	return height;
}

void ProcessPlayerJump(float playerHeight, float scale, Camera* camera)
{
    if(camera->isJumping)
    {
        camera->jumpPower -= GRAVITY / scale;
        camera->position.y += camera->jumpPower / scale;
        if(camera->position.y < playerHeight)
        {
            camera->position.y = playerHeight;
            camera->isJumping = false;
        }
    }
    else
    {
        camera->position.y = playerHeight;
    }
}

void ProcessPlayerMovement(Input* input, Camera* camera, Building* buildings, float deltaTime, float mapHeigt[], AABB collider)
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

    if(GetKeyDown(input, 0x20) && camera->isJumping == false)
    {
        camera->isJumping = true;
        camera->jumpPower = JUMP_POWER;
    }

    Vec3 normPlayerVelocity = {0.0f, 0.0f, 0.0f};
    if(vec3_length(playerVelocity) != 0.0f)
    { 
        normPlayerVelocity = normaliza_vec3(playerVelocity);
    }

    float t = 0;
    Vec3 hitPoint  = {0.0f, 0.0f, 0.0f};
    Vec3 hitNormal = {0.0f, 0.0f, 0.0f};
    bool isGrouded = true;
    float playerOverBoxHeight = 0.0f;
    for(int i = 0; i < 4; i++)
    {
        if(XZRayIntersectAABBX(camera->position, normPlayerVelocity, buildings[i].collider, hitPoint, hitNormal, t) && t <= 1.0f)
        {
            if(TestAABBAABB(collider, buildings[i].collider) == 1)
            {
                Vec3 temp = {absf(normPlayerVelocity.x), normPlayerVelocity.y, absf(normPlayerVelocity.z)};
                if((temp.x == 0 && temp.y == 0 && temp.z == 0) ||
                    (hitNormal.x == 0 && hitNormal.y == 0 && hitNormal.z == 0))
                {
                    normPlayerVelocity = {0.0f, 0.0f, 0.0f};
                }
                else
                {
                    normPlayerVelocity += hitNormal * temp * (1.0f - t);
                }
            }
        }
        if(PointOverAABB(collider.c, buildings[i].collider) == 1 &&
           TestAABBAABB(collider, buildings[i].collider) == 0 &&
           collider.c.y - buildings[i].collider.c.y > 0.0f)
        {
            playerOverBoxHeight = (buildings[i].collider.c.y + buildings[i].collider.r[1]) + 1.1f;
            isGrouded = false;
        }
    }
    
    float playerHeight = 0.0f;
    if(isGrouded) playerHeight = GetEntityHeight(camera->position.x, camera->position.z, mapHeigt, 50, 50) + 1.1f;
    else playerHeight = playerOverBoxHeight;   
    if(camera->position.y > playerHeight) camera->isJumping = true;
    ProcessPlayerJump(playerHeight, 1.0f, camera);
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
                actualEnemy->position   += (enemyVelocity * 1.5f) * deltaTime;
                actualEnemy->collider.c += (enemyVelocity * 1.5f) * deltaTime;
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

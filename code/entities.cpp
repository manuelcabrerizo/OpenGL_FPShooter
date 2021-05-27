#include "entities.h"
#include <math.h>

#include "utility.h"


// temp...
#include <windows.h>
#include <stdio.h>

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

            actualEnemy->model = get_scale_matrix({0.01f, 0.01f, 0.01f}) *
                                 get_rotation_x_matrix(to_radiant(-90.0f)) *
                                 get_rotation_y_matrix(actualEnemy->rotation) *
                                 get_translation_matrix(actualEnemy->position);
        }
        else
        {
            actualEnemy->shouldRender = false;
        }
     }
}

void ShootProjectile(Projectile* projectile, Vec3 start, Vec3 end)
{
    projectile->start = start;
    projectile->end   = end;
    projectile->distance = 0.0f;
    projectile->impactSomething = false;
    projectile->shouldRender = true;
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


#include "entities.h"
#include <math.h>


#include "renderer.h"

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

void InitializeBuildings(Building* buildings,
                         Cube* buildingsMesh,
                         int numberBuildings,
                         Shader* shader,
                         Matrix* buildingsModels[],
                         bool* buildingsShouldRender[])
{
    buildings[0].position = {10.0f, 1.0f, 10.0f};
    buildings[1].position = {-10.0f, 3.0f, -7.0f};
    buildings[2].position = {2.0f, 2.0f, 15.0f};
    buildings[3].position = {-16.0f, 4.0f, 9.0f};
    buildings[0].scale = {4.0f, 1.0f, 2.0f};
    buildings[1].scale = {6.0f, 3.0f, 4.0f};
    buildings[2].scale = {6.0f, 1.5f, 6.0f};
    buildings[3].scale = {5.0f, 4.0f, 2.0f};

    for(int i = 0; i < numberBuildings; i++)
    {
        buildings[i].collider.c = buildings[i].position;
        buildings[i].collider.r[0] = buildings[i].scale.x + 0.1f; 
        buildings[i].collider.r[1] = buildings[i].scale.y;
        buildings[i].collider.r[2] = buildings[i].scale.z + 0.1f;
        buildings[i].model = get_scale_matrix(buildings[i].scale) * get_translation_matrix(buildings[i].position);
        buildings[i].shouldRender = true;
        buildingsModels[i] = &buildings[i].model;
        buildingsShouldRender[i] = &buildings[i].shouldRender;
    }
    PushToRender(buildingsMesh->vao,
                 buildingsMesh->textureID,
                 4,
                 36,
                 false,
                 *shader,
                 buildingsModels,
                 buildingsShouldRender);
}

void InitializeEnemies(Enemy* enemy,
                       Mesh* mesh,
                       int numberEnemies,
                       Shader* shader,
                       Matrix* enemiesModels[],
                       bool* enemyShouldRender[])
{
    // ENEMY::STAFF...
    for(int k = 0; k < 7; k++)
    {
        for(int p = 0; p < 7; p++)
        {
            enemy[(p * 7) + k].position = {(float)p * 1.0f, 0.0f, (float)k * 1.0f};
            enemy[(p * 7) + k].model = get_translation_matrix(enemy[(p * 7) + k].position);
            enemiesModels[(p * 7) + k] = &enemy[(p * 7) + k].model;
            enemy[(p * 7) + k].shouldRender = true;
            enemyShouldRender[(p * 7) + k] = &enemy[(p * 7) + k].shouldRender;
        }
    }
    for(int i = 0; i < numberEnemies; i++)
    {
        enemy[i].collider.c = {enemy[i].position.x,
                               enemy[i].position.y + 0.85f,
                               enemy[i].position.z};
        enemy[i].collider.r[0] = 0.1f; 
        enemy[i].collider.r[1] = 0.8f;
        enemy[i].collider.r[2] = 0.1f;
        enemy[i].life = 1;
    } 
    PushToRender(mesh->vao,
                 mesh->texId,
                 49,
                 mesh->numIndex * 3,
                 true,
                 *shader,
                 enemiesModels,
                 enemyShouldRender); 
}



void InitializeWeapon(Weapon* weapon, Mesh* projectileMesh, Shader* shader, int numberBullets)
{
    for(int i = 0; i < numberBullets; i++)
    {
        weapon->projectile[i].start    = {0.0f, 0.0f, 0.0f};
        weapon->projectile[i].position = {0.0f, 0.0f, 0.0f};
        weapon->projectile[i].end      = {0.0f, 0.0f, 0.0f};
        weapon->projectile[i].speed    = 1.0f;
        weapon->projCollider[i].c = weapon->projectile[i].position;
        weapon->projCollider[i].r[0] = 0.02f;
        weapon->projCollider[i].r[1] = 0.02f;
        weapon->projCollider[i].r[2] = 0.02f;
        weapon->projectile[i].impactSomething = false;
        weapon->projectile[i].model = get_translation_matrix(weapon->projectile[i].position); 
        weapon->projectileModels[i] = &weapon->projectile[i].model;
        weapon->projectile[i].shouldRender = true;
        weapon->projectileShouldRender[i] = &weapon->projectile[i].shouldRender;
    }
    PushToRender(projectileMesh->vao,
                 projectileMesh->texId,
                 numberBullets,
                 projectileMesh->numIndex * 3,
                 true,
                 *shader,
                 weapon->projectileModels,
                 weapon->projectileShouldRender);
}


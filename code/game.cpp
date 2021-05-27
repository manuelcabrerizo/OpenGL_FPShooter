#include <stdio.h>
#include <windows.h>
#include <glad/glad.h>
#include "game.h"
#include "math.h"
#include <math.h>

#include "renderer.h"

void GameInit(MainGame* game)
{    
    LoadShader(&game->main_shader,
            "./code/ColorVertexShader.vert",
            "./code/ColorFragmentShader.frag");
    LoadShader(&game->mesh_shader,
            "./code/MeshVertexShader.vert",
            "./code/MeshFragmentShader.frag");
    LoadShader(&game->ui.shader,
            "./code/UiVertexShader.vert",
            "./code/UiFragmentShader.frag");
    LoadShader(&game->skybox_shader,
            "./code/SkyBoxVertexShader.vert",
            "./code/SkyBoxFragmentShader.frag");

    #include "constants.h"
    for(int i = 0; i < 50*50; i++)
    {
        game->mapHeigt[i] = heights[i];
    }
    

    // Create Boths of our PROJECTION MATRIX...
    Matrix proj = get_projection_perspective_matrix(to_radiant(90), WNDWIDTH/WNDHEIGHT, 0.1f, 100.0f);
    Matrix UI_othogona_proj = get_projection_orthogonal_matrix(WNDWIDTH, WNDHEIGHT, 0.1f, 100.0f); 

    UseShader(&game->ui.shader);
    SetShaderMatrix(UI_othogona_proj, game->ui.shader.projMatLoc);
    UseShader(&game->skybox_shader);
    SetShaderMatrix(proj, game->skybox_shader.projMatLoc);
    UseShader(&game->main_shader);
    SetShaderMatrix(proj, game->main_shader.projMatLoc);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(proj, game->mesh_shader.projMatLoc);
    
    InitializeCamera(&game->player.camera);

    game->ui.xAxis = GenLine({-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, &game->ui.shader);
    game->ui.yAxis = GenLine({0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, &game->ui.shader);
    
    SkyBoxTextures(&game->skyBox,
                   "./data/skybox/ForestRight.bmp",
                   "./data/skybox/ForestLeft.bmp",
                   "./data/skybox/ForestTop.bmp",
                   "./data/skybox/ForestBottom.bmp",
                   "./data/skybox/ForestFront.bmp",
                   "./data/skybox/ForestBack.bmp"); 
    GenerateSkyBox(&game->skyBox);
    GenerateCube(&game->colliderCube, "./data/mud.bmp");
    GenerateTerrain(&game->terrain, -25, -25, 50, 50, 1, game->mapHeigt,"./data/grass0.bmp");
    LoadOBJFileIndex(&game->pistol, "./data/weapon.obj", "./data/weapon.bmp");
    LoadOBJFileIndex(&game->ball, "./data/bullet.obj", "./data/wall.bmp");
    LoadOBJFileIndex(&game->naruto, "./data/naruto.obj", "./data/naruto.bmp");
    LoadOBJFileIndex(&game->colliderMesh, "./data/collider.obj", "./data/pistol.bmp");

    game->player.transform.collider.c = game->player.transform.position;
    game->player.transform.collider.r[0] = 0.3f; 
    game->player.transform.collider.r[1] = 1.0f;
    game->player.transform.collider.r[2] = 0.3f;

    game->buildings[0].position = {10.0f, 1.0f, 10.0f};
    game->buildings[1].position = {-10.0f, 3.0f, -7.0f};
    game->buildings[2].position = {2.0f, 2.0f, 15.0f};
    game->buildings[3].position = {-16.0f, 4.0f, 9.0f};
    game->buildings[0].scale = {4.0f, 1.0f, 2.0f};
    game->buildings[1].scale = {6.0f, 3.0f, 4.0f};
    game->buildings[2].scale = {6.0f, 1.5f, 6.0f};
    game->buildings[3].scale = {5.0f, 4.0f, 2.0f};
    for(int i = 0; i < 4; i++)
    {
        game->buildings[i].collider.c = game->buildings[i].position;
        game->buildings[i].collider.r[0] = game->buildings[i].scale.x + 0.1f; 
        game->buildings[i].collider.r[1] = game->buildings[i].scale.y;
        game->buildings[i].collider.r[2] = game->buildings[i].scale.z + 0.1f;
        
        game->buildings[i].model = get_scale_matrix(game->buildings[i].scale) * get_translation_matrix(game->buildings[i].position);
    }
    
    for(int i = 0; i < 200; i++)
    {
        game->player.weapon.projectile[i].start    = {0.0f, 0.0f, 0.0f};
        game->player.weapon.projectile[i].position = {0.0f, 0.0f, 0.0f};
        game->player.weapon.projectile[i].end      = {0.0f, 0.0f, 0.0f};
        game->player.weapon.projectile[i].speed    = 1.0f;
        game->player.weapon.projCollider[i].c = game->player.weapon.projectile[i].position;
        game->player.weapon.projCollider[i].r[0] = 0.02f;
        game->player.weapon.projCollider[i].r[1] = 0.02f;
        game->player.weapon.projCollider[i].r[2] = 0.02f;
        game->player.weapon.projectile[i].impactSomething = false;
        game->player.weapon.projectile[i].model = get_translation_matrix(game->player.weapon.projectile[i].position); 
        game->projectileModels[i] = &game->player.weapon.projectile[i].model;
    }

    // ENEMY::STAFF...
    for(int k = 0; k < 7; k++)
    {
        for(int p = 0; p < 7; p++)
        {
            game->enemy[(p * 7) + k].position = {(float)p * 1.0f, 0.0f, (float)k * 1.0f};
            game->enemy[(p * 7) + k].model = get_translation_matrix(game->enemy[(p * 7) + k].position);
            game->enemiesModels[(p * 7) + k] = &game->enemy[(p * 7) + k].model;
        }
    }
    for(int i = 0; i < 49; i++)
    {
        game->enemy[i].collider.c = {game->enemy[i].position.x,
                                     game->enemy[i].position.y + 0.85f,
                                     game->enemy[i].position.z};
        game->enemy[i].collider.r[0] = 0.1f; 
        game->enemy[i].collider.r[1] = 0.8f;
        game->enemy[i].collider.r[2] = 0.1f;
        game->enemy[i].life = 1;
    }
    
    game->terrainModels[0] = &game->terrain.model; 
    PushToRender(game->terrain.vao,
                 game->terrain.texId,
                 1,
                 game->terrain.numIndex,
                 true,
                 game->mesh_shader,
                 game->terrainModels);

    for(int i = 0; i < 4; i++)
    {
        game->buildingsModels[i] = &game->buildings[i].model;
    } 
    PushToRender(game->colliderCube.vao,
                 game->colliderCube.textureID,
                 4,
                 36,
                 false,
                 game->mesh_shader,
                 game->buildingsModels);

    game->weaponModel[0] =  &game->player.weapon.model;
    PushToRender(game->pistol.vao,
                 game->pistol.texId,
                 1,
                 game->pistol.numIndex * 3,
                 true,
                 game->mesh_shader,
                 game->weaponModel);

    PushToRender(game->ball.vao,
                 game->ball.texId,
                 200,
                 game->ball.numIndex * 3,
                 true,
                 game->mesh_shader,
                 game->projectileModels);

    PushToRender(game->naruto.vao,
                 game->naruto.texId,
                 49,
                 game->naruto.numIndex * 3,
                 true,
                 game->mesh_shader,
                 game->enemiesModels);     
}

void GameUnpdateAndRender(MainGame* game, float deltaTime)
{
    // Update... 
    UpdateCamera(&game->player.camera, &game->input, deltaTime);
    ProcessPlayerMovementTest(&game->player, &game->input, deltaTime);
    ProcessPlayerCollitions(&game->player, game->buildings, game->mapHeigt, deltaTime);
    ProcessEnemyMovementAndCollition(game->enemy, 49, game->buildings, 4, &game->player.camera, deltaTime);
    game->player.weapon.model = ProcessPlayerWeapon(&game->player.weapon, &game->player.camera, &game->input, deltaTime);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(game->player.camera.viewMat, game->mesh_shader.viewMatLoc);
    UseShader(&game->main_shader);
    SetShaderMatrix(game->player.camera.viewMat, game->main_shader.viewMatLoc);
    
    for(int i = 0; i < 49; i++)
    {
        if(game->enemy[i].life > 0)
        { 
            game->enemy[i].model = get_scale_matrix({0.01f, 0.01f, 0.01f}) *
                                   get_rotation_x_matrix(to_radiant(-90.0f)) *
                                   get_rotation_y_matrix(game->enemy[i].rotation) *
                                   get_translation_matrix(game->enemy[i].position);
        }
    }
    
    for(int i = 0 ; i < 200; i++)
    {
        Projectile* actualProjectile = &game->player.weapon.projectile[i];
        AABB* actualProjCollider = &game->player.weapon.projCollider[i];

        game->player.weapon.projectile[i].model = get_scale_matrix({0.05f, 0.05f, 0.05f}) * UpdateProjectile(actualProjectile, deltaTime);
        actualProjCollider->c = actualProjectile->position;
        if(actualProjectile->distance <= 1.0f && actualProjectile->impactSomething == false)
        {
            float t = 0;
            Vec3 hitPoint  = {0.0f, 0.0f, 0.0f};
            Vec3 hitNormal = {0.0f, 0.0f, 0.0f}; 
            for(int j = 0; j < 4; j++)
            {
                Vec3 direction = actualProjectile->end - actualProjectile->start;
                if(XZRayIntersectAABBX(actualProjectile->start, direction, game->buildings[j].collider, hitPoint, hitNormal, t) == 1 && t >= 0.0f && t <= 1.0f)
                { 
                    if(actualProjectile->distance > t)
                    {
                        if(TestAABBAABB(*actualProjCollider, game->buildings[j].collider) == 1 && actualProjectile->impactSomething == false)
                        {
                            Vec3 newTarget = normaliza_vec3(Vec3Reflect(actualProjectile->start, actualProjectile->end, hitNormal));
                            ShootProjectile(actualProjectile, actualProjectile->position, actualProjectile->position + (newTarget * 20.0f));
                        }
                    }               
                }          
            }
            for(int j = 0; j < 49; j++)
            {
                if(game->enemy[j].life > 0)
                {
                    if(TestAABBAABB(*actualProjCollider, game->enemy[j].collider) == 1 && actualProjectile->impactSomething == false)
                    {
                        game->enemy[j].life--;
                        actualProjectile->impactSomething = true;
                    }
                }
            }
        }
    }
    // Render...   
    // UI STAFF...
    float xPos = (float)WNDWIDTH  / 2.0f;
    float yPos = (float)WNDHEIGHT / 2.0f;
    DrawLine(&game->ui.xAxis, get_scale_matrix({10.0f, 0.0f, 0.0f}) * get_translation_matrix({xPos, yPos, 0.0f})); 
    DrawLine(&game->ui.yAxis, get_scale_matrix({0.0f, 10.0f, 0.0f}) * get_translation_matrix({xPos, yPos, 0.0f}));
    // GAME 3D STUFF...
    // SKYBOX DRAW TESTING::THIS CAN BE BAD...
    glDepthMask(GL_FALSE);
    UseShader(&game->skybox_shader);
    Matrix SkyBoxView = to_4x4_matrix(to_3x3_matrix(game->player.camera.viewMat));
    SetShaderMatrix(SkyBoxView, game->skybox_shader.viewMatLoc);
    Matrix SkyBoxWorld = get_identity_matrix();
    SetShaderMatrix(SkyBoxWorld, game->skybox_shader.worldMatLoc);
    glBindVertexArray(game->skyBox.vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, game->skyBox.textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    RenderRendererBuffer();
}




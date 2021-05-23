#include <stdio.h>
#include <windows.h>
#include <glad/glad.h>
#include "game.h"
#include "math.h"
#include <math.h>

void GameInit(MainGame* game)
{    
    LoadShader(&game->main_shader,
            "./code/vertexShader.vert",
            "./code/fragmentShader.frag");
    LoadShader(&game->mesh_shader,
            "./code/sphereVertexShader.vert",
            "./code/sphereFragmentShader.frag");
    LoadShader(&game->ui.shader,
            "./code/uiVertexShader.vert",
            "./code/uiFragmentShader.frag");
    // Create Boths of our PROJECTION MATRIX...
    Matrix proj = get_projection_perspective_matrix(to_radiant(90), WNDWIDTH/WNDHEIGHT, 0.1f, 100.0f);
    Matrix UI_othogona_proj = get_projection_orthogonal_matrix(WNDWIDTH, WNDHEIGHT, 0.1f, 100.0f); 

    UseShader(&game->ui.shader);
    SetShaderMatrix(UI_othogona_proj, game->ui.shader.projMatLoc);
    UseShader(&game->main_shader);
    SetShaderMatrix(proj, game->main_shader.projMatLoc);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(proj, game->mesh_shader.projMatLoc);
    InitializeCamera(&game->camera);

    game->ui.xAxis = GenLine({-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, &game->ui.shader);
    game->ui.yAxis = GenLine({0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, &game->ui.shader);
    game->xAxis = GenLine({-100.0f, 0.0f, 0.0f}, {100.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, &game->main_shader);  
    game->yAxis = GenLine({0.0f, -100.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, &game->main_shader);
    game->zAxis = GenLine({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 100.0f}, {0.0f, 0.0f, 1.0f}, &game->main_shader); 
    
    GenerateTerrain(&game->terrain, -25, -25, 50, 50, 1, "./data/terrain.bmp");
    LoadOBJFileIndex(&game->pistol, "./data/weapon.obj", "./data/weapon.bmp");
    LoadOBJFileIndex(&game->ball, "./data/bullet.obj", "./data/bullet.bmp");
    LoadOBJFileIndex(&game->naruto, "./data/naruto.obj", "./data/naruto.bmp");
    LoadOBJFileIndex(&game->colliderMesh, "./data/collider.obj", "./data/pistol.bmp");


    game->buildings[0].position = {10.0f, 1.0f, 10.0f};
    game->buildings[1].position = {-10.0f, 3.0f, -7.0f};
    game->buildings[2].position = {-2.0f, 2.0f, 15.0f};
    game->buildings[3].position = {-16.0f, 4.0f, 9.0f};
    game->buildings[0].scale = {4.0f, 1.0f, 2.0f};
    game->buildings[1].scale = {6.0f, 3.0f, 4.0f};
    game->buildings[2].scale = {6.0f, 2.0f, 6.0f};
    game->buildings[3].scale = {5.0f, 4.0f, 2.0f};
    for(int i = 0; i < 4; i++)
    {
        game->buildings[i].collider.c = game->buildings[i].position;
        game->buildings[i].collider.r[0] = game->buildings[i].scale.x; 
        game->buildings[i].collider.r[1] = game->buildings[i].scale.y;
        game->buildings[i].collider.r[2] = game->buildings[i].scale.z;
    }
    
    for(int i = 0; i < 200; i++)
    {
        game->projectile[i].start    = {0.0f, 0.0f, 0.0f};
        game->projectile[i].position = {0.0f, 0.0f, 0.0f};
        game->projectile[i].end      = {0.0f, 0.0f, 0.0f};
        game->projectile[i].speed    = 2.0f;
        game->projCollider[i].c = game->projectile[i].position;
        game->projCollider[i].r[0] = 0.02f;
        game->projCollider[i].r[1] = 0.02f;
        game->projCollider[i].r[2] = 0.02f;
        game->projectile[i].impactSomething = false;
    }

    // ENEMY::STAFF...
    game->enemy[0].position = {5.0f, 0.0f, 3.0f};
    game->enemy[1].position = {-5.0f, 0.0f, 3.0f};
    game->enemy[2].position = {-5.0f, 0.0f, -3.0f};
    game->enemy[3].position = {2.0f, 0.0f, -1.0f};
    game->enemy[4].position = {-5.0f, 0.0f, 7.0f};
    for(int i = 0; i < 5; i++)
    {
        game->enemy[i].collider.c = {game->enemy[i].position.x,
                                     game->enemy[i].position.y + 0.85f,
                                     game->enemy[i].position.z};
        game->enemy[i].collider.r[0] = 0.2f; 
        game->enemy[i].collider.r[1] = 0.8f;
        game->enemy[i].collider.r[2] = 0.2f;
        game->enemy[i].life = 5;
    }

}

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

void GameUnpdateAndRender(MainGame* game, float deltaTime)
{
    // Update...
    UpdateCamera(&game->camera, &game->input, deltaTime);
    ProcessPlayerMovement(&game->input, &game->camera, game->buildings, deltaTime);

    UseShader(&game->main_shader);
    SetShaderMatrix(game->camera.viewMat, game->main_shader.viewMatLoc);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(game->camera.viewMat, game->mesh_shader.viewMatLoc);
    // Render...   
    // UI STAFF...
    float xPos = (float)WNDWIDTH  / 2.0f;
    float yPos = (float)WNDHEIGHT / 2.0f;
    DrawLine(&game->ui.xAxis, get_scale_matrix({10.0f, 0.0f, 0.0f}) * get_translation_matrix({xPos, yPos, 0.0f})); 
    DrawLine(&game->ui.yAxis, get_scale_matrix({0.0f, 10.0f, 0.0f}) * get_translation_matrix({xPos, yPos, 0.0f}));
    // GAME 3D STUFF...
    DrawLine(&game->xAxis, get_identity_matrix());
    DrawLine(&game->yAxis, get_identity_matrix());
    DrawLine(&game->zAxis, get_identity_matrix());
    
    Matrix model = get_identity_matrix();
    UseShader(&game->mesh_shader);
    for(int i = 0; i < 5; i++)
    {
        if(game->enemy[i].life > 0)
        { 
            model = get_scale_matrix({0.01f, 0.01f, 0.01f}) * get_rotation_x_matrix(to_radiant(-90.0f)) * get_translation_matrix(game->enemy[i].position);
            glBindTexture(GL_TEXTURE_2D, game->naruto.texId);
            glBindVertexArray(game->naruto.vao);
            SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->naruto.numIndex * 3, GL_UNSIGNED_INT, 0);

            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(game->colliderMesh.vao);
            glBindTexture(GL_TEXTURE_2D, game->colliderMesh.texId);
            model = get_scale_matrix({0.2f, 0.8f, 0.2f}) * get_translation_matrix(game->enemy[i].collider.c);
            SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->colliderMesh.numIndex * 3, GL_UNSIGNED_INT, 0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    
    glBindVertexArray(game->colliderMesh.vao);
    for(int i = 0; i < 4; i++)
    {
        glBindTexture(GL_TEXTURE_2D, game->ball.texId);
        model = get_scale_matrix(game->buildings[i].scale) * get_translation_matrix(game->buildings[i].position);
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        glDrawElements(GL_TRIANGLES, game->colliderMesh.numIndex * 3, GL_UNSIGNED_INT, 0);

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
        glBindTexture(GL_TEXTURE_2D, game->colliderMesh.texId);
        model = get_scale_matrix(game->buildings[i].scale) * get_translation_matrix(game->buildings[i].collider.c);
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        glDrawElements(GL_TRIANGLES, game->colliderMesh.numIndex * 3, GL_UNSIGNED_INT, 0);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    }
    
    model = get_identity_matrix();
    glBindTexture(GL_TEXTURE_2D, game->terrain.texId);
    glBindVertexArray(game->terrain.vao);
    SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->terrain.numIndex, GL_UNSIGNED_INT, 0);

    glBindVertexArray(game->pistol.vao);
    glBindTexture(GL_TEXTURE_2D, game->pistol.texId);
    
    Vec3 zOffset = normaliza_vec3(game->camera.target);
    Vec3 xOffset = normaliza_vec3(game->camera.right);
    Vec3 pistolFinalPos = game->camera.position + zOffset*0.2f + xOffset*0.05f; 

    if(game->camera.isMoving == true)
    {  
        static float time = 0.0f;
        pistolFinalPos.y = pistolFinalPos.y + sinf(time) / 50.0f;
        time += deltaTime * 15.0f;
    } 
    
    Matrix pistolPos = get_translation_matrix(pistolFinalPos);
    Matrix pistolRot = get_rotation_y_matrix(-to_radiant(game->camera.yaw + -90.0f)) *
                       get_rotation_arbitrary_matrix(xOffset, to_radiant(game->camera.pitch));
    Matrix pistolModel = get_scale_matrix({0.01f, 0.01f, 0.01f}) * pistolRot * pistolPos;


    SetShaderMatrix(pistolModel, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->pistol.numIndex * 3, GL_UNSIGNED_INT, 0);

    // START::PROCCESING::OUR::PROJECTILE::STUFF...
    static bool mouseLButtonPress = false;
    static int actualProjectile   = 0;
    if((GetMouseButtonPress(&game->input, LEFTBUTTON) || game->input.A == true) && mouseLButtonPress == false)
    { 
        Vec3 bulletPos = pistolFinalPos;
        bulletPos.y -= 0.03f;
        ShootProjectile(&game->projectile[actualProjectile], bulletPos, game->camera.position + (normaliza_vec3(game->camera.target) * 20.0f));
        mouseLButtonPress = true;
        actualProjectile++;
    }
    if((!GetMouseButtonPress(&game->input, LEFTBUTTON) && game->input.A == false) && mouseLButtonPress == true)
    {
        mouseLButtonPress = false;
    }
    actualProjectile %= 200;

    for(int i = 0 ; i < 200; i++)
    {
        Matrix projectileModel = get_scale_matrix({0.02f, 0.02f, 0.02f}) * UpdateProjectile(&game->projectile[i], deltaTime);
        game->projCollider[i].c = game->projectile[i].position;
        if(game->projectile[i].distance <= 1.0f && game->projectile[i].impactSomething == false)
        {
            glBindVertexArray(game->ball.vao);
            glBindTexture(GL_TEXTURE_2D, game->ball.texId);
            SetShaderMatrix(projectileModel, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0);

            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(game->colliderMesh.vao);
            glBindTexture(GL_TEXTURE_2D, game->colliderMesh.texId);
            projectileModel = get_scale_matrix({0.02f, 0.02f, 0.02f}) * get_translation_matrix(game->projCollider[i].c);
            SetShaderMatrix(projectileModel, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->colliderMesh.numIndex * 3, GL_UNSIGNED_INT, 0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);


            float t = 0;
            Vec3 hitPoint  = {0.0f, 0.0f, 0.0f};
            Vec3 hitNormal = {0.0f, 0.0f, 0.0f}; 
            for(int j = 0; j < 4; j++)
            {
                Vec3 direction = game->projectile[i].end - game->projectile[i].start;
                if(XZRayIntersectAABBX(game->projectile[i].start, direction, game->buildings[j].collider, hitPoint, hitNormal, t) == 1 && t <= 1.0f)
                {
                    if(game->projectile[i].distance <= 1.0f)
                    {
                        if(t >= 0.0f && t <= 1.0f)
                        {
                            if(game->projectile[i].distance > t)
                            {
                                if(TestAABBAABB(game->projCollider[i], game->buildings[j].collider) == 1 && game->projectile[i].impactSomething == false)
                                {
                                    Vec3 newTarget = normaliza_vec3(Vec3Reflect(game->projectile[i].start, game->projectile[i].end, hitNormal));
                                    ShootProjectile(&game->projectile[i], game->projectile[i].position, game->projectile[i].position + (newTarget * 20.0f));
                                }
                            }  
                        }
                    }           
                }          
            }
            for(int j = 0; j < 5; j++)
            {
                if(game->enemy[j].life > 0)
                {
                    if(TestAABBAABB(game->projCollider[i], game->enemy[j].collider) == 1 && game->projectile[i].impactSomething == false)
                    {
                        game->enemy[j].life--;
                        game->projectile[i].impactSomething = true;
                    }
                }
            }
        }
    }


}




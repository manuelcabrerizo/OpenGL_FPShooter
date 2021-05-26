#include <stdio.h>
#include <windows.h>
#include <glad/glad.h>
#include "game.h"
#include "math.h"
#include <math.h>

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
    /*
    // Temoporal::heightMap::Loading::for::the::position::of::the::entities
    Texture mapHeight = LoadBMP("./data/terrain.bmp");
    for(int y = 0; y < 256; y++)
    {
        for(int x = 0; x < 256; x++)
        {
            uint32_t pixel = mapHeight.pixels[(y * 255) + x];
            uint8_t height = uint8_t(pixel & 0xFFFFFF);
            game->mapHeigt[(y * 255) + x] = (float)height * 0.1f;
        }
    }
    free(mapHeight.pixels); 
    // --------------------------------------------------------------------
    */
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
    InitializeCamera(&game->camera);

    game->ui.xAxis = GenLine({-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, &game->ui.shader);
    game->ui.yAxis = GenLine({0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, &game->ui.shader);
    game->xAxis = GenLine({-100.0f, 0.0f, 0.0f}, {100.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, &game->main_shader);  
    game->yAxis = GenLine({0.0f, -100.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, &game->main_shader);
    game->zAxis = GenLine({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 100.0f}, {0.0f, 0.0f, 1.0f}, &game->main_shader); 
    
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


    game->playerCollider.c = game->camera.position;
    game->playerCollider.r[0] = 0.3f; 
    game->playerCollider.r[1] = 1.0f;
    game->playerCollider.r[2] = 0.3f;

    game->buildings[0].position = {10.0f, 1.0f, 10.0f};
    game->buildings[1].position = {-10.0f, 3.0f, -7.0f};
    game->buildings[2].position = {0.0f, 2.0f, 15.0f};
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
    }
    
    for(int i = 0; i < 200; i++)
    {
        game->projectile[i].start    = {0.0f, 0.0f, 0.0f};
        game->projectile[i].position = {0.0f, 0.0f, 0.0f};
        game->projectile[i].end      = {0.0f, 0.0f, 0.0f};
        game->projectile[i].speed    = 1.0f;
        game->projCollider[i].c = game->projectile[i].position;
        game->projCollider[i].r[0] = 0.02f;
        game->projCollider[i].r[1] = 0.02f;
        game->projCollider[i].r[2] = 0.02f;
        game->projectile[i].impactSomething = false;
    }

    // ENEMY::STAFF...
    for(int k = 0; k < 7; k++)
    {
        for(int p = 0; p < 7; p++)
        {
            game->enemy[(p * 7) + k].position = {(float)p * 1.0f, 0.0f, (float)k * 1.0f};
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
}

void GameUnpdateAndRender(MainGame* game, float deltaTime)
{
    // Update...
    UpdateCamera(&game->camera, &game->input, deltaTime);
    game->playerCollider.c = game->camera.position;
    ProcessPlayerMovement(&game->input, &game->camera, game->buildings, deltaTime, game->mapHeigt, game->playerCollider);
    ProcessEnemyMovementAndCollition(game->enemy, 49, game->buildings, 4, &game->camera, deltaTime);
    


    // SKYBOX DRAW TESTING:::::THIS CAN BE BADD...
    glDepthMask(GL_FALSE);
    UseShader(&game->skybox_shader);
    Matrix SkyBoxView = to_4x4_matrix(to_3x3_matrix(game->camera.viewMat));
    SetShaderMatrix(SkyBoxView, game->skybox_shader.viewMatLoc);
    Matrix SkyBoxWorld = get_identity_matrix();
    SetShaderMatrix(SkyBoxWorld, game->skybox_shader.worldMatLoc);
    glBindVertexArray(game->skyBox.vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, game->skyBox.textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

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
    for(int i = 0; i < 49; i++)
    {
        if(game->enemy[i].life > 0)
        { 
            model = get_scale_matrix({0.01f, 0.01f, 0.01f}) *
                    get_rotation_x_matrix(to_radiant(-90.0f)) *
                    get_rotation_y_matrix(game->enemy[i].rotation) *
                    get_translation_matrix(game->enemy[i].position);
            glBindTexture(GL_TEXTURE_2D, game->naruto.texId);
            glBindVertexArray(game->naruto.vao);
            SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->naruto.numIndex * 3, GL_UNSIGNED_INT, 0);

            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(game->colliderMesh.vao);
            glBindTexture(GL_TEXTURE_2D, game->colliderMesh.texId);
            model = get_scale_matrix({0.1f, 0.8f, 0.1f}) * get_translation_matrix(game->enemy[i].collider.c);
            SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
            //glDrawElements(GL_TRIANGLES, game->colliderMesh.numIndex * 3, GL_UNSIGNED_INT, 0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
        }
    }


    
    glBindVertexArray(game->colliderCube.vao);
    for(int i = 0; i < 4; i++)
    {
        glBindTexture(GL_TEXTURE_2D, game->colliderCube.textureID);
        model = get_scale_matrix(game->buildings[i].scale) * get_translation_matrix(game->buildings[i].position);
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
        glBindTexture(GL_TEXTURE_2D, game->colliderMesh.texId);
        model = get_scale_matrix(game->buildings[i].scale) * get_translation_matrix(game->buildings[i].collider.c);
        SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    }
    /*
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    glBindTexture(GL_TEXTURE_2D, game->colliderMesh.texId);
    model = get_scale_matrix({0.3f, 1.0f, 0.3f}) * get_translation_matrix(game->playerCollider.c);
    SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    */
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
                if(XZRayIntersectAABBX(game->projectile[i].start, direction, game->buildings[j].collider, hitPoint, hitNormal, t) == 1 && t >= 0.0f && t <= 1.0f)
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
            for(int j = 0; j < 49; j++)
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




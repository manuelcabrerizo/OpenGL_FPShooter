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
    
    GenerateTerrain(&game->terrain, -10, -10, 20, 20, 1, "./data/terrain.bmp");
    LoadOBJFileIndex(&game->pistol, "./data/weapon.obj", "./data/pistol.bmp");
    LoadOBJFileIndex(&game->ball, "./data/bullet.obj", "./data/bullet.bmp");
    LoadOBJFileIndex(&game->naruto, "./data/naruto.obj", "./data/naruto.bmp");
    LoadOBJFileIndex(&game->enemy.colliderMesh, "./data/collider.obj", "./data/pistol.bmp");

    for(int i = 0; i < 200; i++)
    {
        game->projectile[i].start    = {0.0f, 0.0f, 0.0f};
        game->projectile[i].position = {0.0f, 0.0f, 0.0f};
        game->projectile[i].end      = {0.0f, 0.0f, 0.0f};
        game->projectile[i].speed    = 2.0f;
        game->projCollider[i].c = game->projectile[i].position;
        game->projCollider[i].r[0] = 0.01f;
        game->projCollider[i].r[1] = 0.01f;
        game->projCollider[i].r[2] = 0.01f;
    }

    // ENEMY::STAFF...
    game->enemy.position = {10.0f, 0.0f, 10.0f};
    game->enemy.collider.c = {game->enemy.position.x,
                              game->enemy.position.y + 0.8f,
                              game->enemy.position.z};
    game->enemy.collider.r[0] = 0.2f; 
    game->enemy.collider.r[1] = 0.8f;
    game->enemy.collider.r[2] = 0.2f;

}

void ShootProjectile(Projectile* projectile, Vec3 start, Vec3 end)
{
    projectile->start = start;
    projectile->end   = end;
    projectile->distance = 0.0f;
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

    UseShader(&game->mesh_shader);
    Matrix model = get_scale_matrix({0.01f, 0.01f, 0.01f}) * get_rotation_x_matrix(to_radiant(-90.0f)) * get_translation_matrix(game->enemy.position);
    glBindTexture(GL_TEXTURE_2D, game->naruto.texId);
    glBindVertexArray(game->naruto.vao);
    SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->naruto.numIndex * 3, GL_UNSIGNED_INT, 0);
    
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
        pistolFinalPos.y = pistolFinalPos.y + sinf(time) / 40.0f;
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
    if(GetMouseButtonPress(&game->input, LEFTBUTTON) && mouseLButtonPress == false)
    { 
        Vec3 bulletPos = pistolFinalPos;
        bulletPos.y -= 0.03f;
        ShootProjectile(&game->projectile[actualProjectile], bulletPos, game->camera.position + (game->camera.target * 10.0f));
        mouseLButtonPress = true;
        actualProjectile++;
    }
    if(!GetMouseButtonPress(&game->input, LEFTBUTTON) && mouseLButtonPress == true)
    {
        mouseLButtonPress = false;
    }
    actualProjectile %= 200;
    for(int i = 0 ; i < 200; i++)
    {
        Matrix projectileModel = get_scale_matrix({0.01f, 0.01f, 0.01f}) * UpdateProjectile(&game->projectile[i], deltaTime);
        game->projCollider[i].c = game->projectile[i].position;
        if(game->projectile[i].distance <= 1.0f)
        {
            glBindVertexArray(game->ball.vao);
            glBindTexture(GL_TEXTURE_2D, game->ball.texId);
            SetShaderMatrix(projectileModel, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0);
        }

        if(TestAABBAABB(game->projCollider[i], game->enemy.collider) == 1)
        {
            OutputDebugString("shoot!!\n");
        }
    }


}

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
    LoadOBJFileIndex(&game->pistol, "./data/gun.obj", "./data/pistol.bmp");
    LoadOBJFileIndex(&game->ball, "./data/bullet.obj", "./data/bullet.bmp");

    for(int i = 0; i < 200; i++)
    {
        game->projectile[i].start    = {0.0f, 0.0f, 0.0f};
        game->projectile[i].position = {0.0f, 0.0f, 0.0f};
        game->projectile[i].end      = {0.0f, 0.0f, 0.0f};
        game->projectile[i].speed    = 2.0f;
    }

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
    DrawLine(&game->ui.xAxis, get_scale_matrix({30.0f, 0.0f, 0.0f}) * get_translation_matrix({xPos, yPos, 0.0f})); 
    DrawLine(&game->ui.yAxis, get_scale_matrix({0.0f, 30.0f, 0.0f}) * get_translation_matrix({xPos, yPos, 0.0f}));
    // GAME 3D STUFF...
    DrawLine(&game->xAxis, get_identity_matrix());
    DrawLine(&game->yAxis, get_identity_matrix());
    DrawLine(&game->zAxis, get_identity_matrix());

    UseShader(&game->mesh_shader);
    glBindTexture(GL_TEXTURE_2D, game->terrain.texId);
    glBindVertexArray(game->terrain.vao);
    Matrix model = get_identity_matrix();
    SetShaderMatrix(model, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->terrain.numIndex, GL_UNSIGNED_INT, 0);

    glBindVertexArray(game->pistol.vao);
    glBindTexture(GL_TEXTURE_2D, game->pistol.texId);
    
    
    //######################-IMPROVE-######################
    Vec3 pistolPosition = {1.0f, 1.0f, 1.0f};
    pistolPosition = normaliza_vec3(vec3_rotate_arbitrary_axis(game->camera.right, pistolPosition, to_radiant(game->camera.pitch)));
    pistolPosition = normaliza_vec3(vec3_rotate_arbitrary_axis(game->camera.up, pistolPosition, to_radiant(game->camera.yaw)));
    if(game->camera.isMoving == true)
    {  
        static float time = 0.0f;
        pistolPosition.y = pistolPosition.y + sinf(time) / 40.0f;
        time += deltaTime * 15.0f;
    }
    pistolPosition.y -= 1.0f;
    //######################################################

     
    Matrix pistolPos = get_translation_matrix(game->camera.position + pistolPosition*0.2f);
    Matrix pistolRot = get_rotation_y_matrix(-to_radiant(game->camera.yaw + -90.0f)) *
                       get_rotation_arbitrary_matrix(game->camera.right, to_radiant(game->camera.pitch));
    Matrix pistolModel = get_scale_matrix({0.01f, 0.01f, 0.01f}) * pistolRot * pistolPos;


    SetShaderMatrix(pistolModel, game->mesh_shader.worldMatLoc);
    glDrawElements(GL_TRIANGLES, game->pistol.numIndex * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(game->ball.vao);
    glBindTexture(GL_TEXTURE_2D, game->ball.texId);
    // START::PROCCESING::OUR::PROJECTILE::STUFF...
    static bool mouseLButtonPress = false;
    static int actualProjectile   = 0;
    if(GetMouseButtonPress(&game->input, LEFTBUTTON) && mouseLButtonPress == false)
    {
        ShootProjectile(&game->projectile[actualProjectile], game->camera.position + pistolPosition*0.2f, game->camera.position + (game->camera.target * 10.0f));
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
        Matrix projectileModel = get_scale_matrix({0.02f, 0.02f, 0.02f}) * UpdateProjectile(&game->projectile[i], deltaTime);
        if(game->projectile[i].distance <= 1.0f)
        {
            SetShaderMatrix(projectileModel, game->mesh_shader.worldMatLoc);
            glDrawElements(GL_TRIANGLES, game->ball.numIndex * 3, GL_UNSIGNED_INT, 0);
        }
    }


}

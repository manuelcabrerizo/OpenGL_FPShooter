#include <stdio.h>
#include <windows.h>
#include <glad/glad.h>
#include "game.h"
#include "math.h"
#include <math.h>
#include "colladaParser.h"

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
    LoadShader(&game->animate_shader,
            "./code/AnimateVertexShader.vert",
            "./code/AnimateFragmentShader.frag");
    LoadColladaFile(&game->colladaVao,
                    &game->colladaTexId,
                    &game->animation,
                    &game->am.rootJoint,
                    &game->am.jointCount,
                    &game->colladaMeshNumVertex, 
                    "./data/model.dae",
                    "./data/cowboy.bmp",
                    true);
 
    InitAnimatedModel(&game->am, game->colladaVao, game->colladaTexId);
    InitAnimator(&game->animator, &game->am);
    DoAnimation(&game->animator, &game->animation);
    //game->matrixArray = GetAnimatedModelPose(&game->animation, 1);

    

    

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
    UseShader(&game->animate_shader);
    SetShaderMatrix(proj, game->animate_shader.projMatLoc);
    UseShader(&game->mesh_shader);
    SetShaderMatrix(proj, game->mesh_shader.projMatLoc);
  

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
    
    InitializePlayer(&game->player, &game->pistol, &game->ball, &game->mesh_shader);
    InitializeBuildings(game->buildings, &game->colliderCube, 4, &game->mesh_shader, game->buildingsModels, game->buildingsShouldRender);
    InitializeEnemies(game->enemy, &game->naruto, 49, &game->mesh_shader, game->enemiesModels, game->enemyShouldRender);

    game->terrainModels[0] = &game->terrain.model;
    game->terrainShouldRender[0] = &game->terrain.shouldRender; 
    PushToRender(game->terrain.vao, game->terrain.texId, 1, game->terrain.numIndex, true, game->mesh_shader, game->terrainModels, game->terrainShouldRender); 
}

void GameUnpdateAndRender(MainGame* game, float deltaTime)
{
    // Update... 
    UpdateCamera(&game->player.camera, &game->input, deltaTime);
    ProcessPlayerMovementTest(&game->player, &game->input, deltaTime);
    ProcessPlayerCollitions(&game->player, game->buildings, game->mapHeigt, deltaTime);
    ProcessEnemyMovementAndCollition(game->enemy, 49, game->buildings, 4, &game->player.camera, deltaTime);
    ProcessPlayerProjectiles(&game->player, game->buildings, game->enemy, deltaTime);
    game->player.weapon.model = ProcessPlayerWeapon(&game->player.weapon, &game->player.camera, &game->input, deltaTime);
    AnimatorUpdate(&game->animator, deltaTime);

    UseShader(&game->mesh_shader);
    SetShaderMatrix(game->player.camera.viewMat, game->mesh_shader.viewMatLoc);
    UseShader(&game->animate_shader);
    SetShaderMatrix(game->player.camera.viewMat, game->animate_shader.viewMatLoc);
    UseShader(&game->main_shader);
    SetShaderMatrix(game->player.camera.viewMat, game->main_shader.viewMatLoc);
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
    
    UseShader(&game->mesh_shader);
    RenderRendererBuffer();

    // PRIMER::PRUEVA::DE::MODELO::CARGADO::CON::COLLADA::FILE::TYPE...
    UseShader(&game->animate_shader); 
    game->matrixArray = GetJointTransforms(&game->am);    
    unsigned int MatArrayLoc = glGetUniformLocation(game->animate_shader.id, "jointTransforms");
    SetShaderMatrixArray(game->matrixArray, MatArrayLoc, 256); 
    SetShaderMatrix(get_identity_matrix(),
                    game->mesh_shader.worldMatLoc);
    glBindVertexArray(game->am.vao);
    glBindTexture(GL_TEXTURE_2D, game->am.textId);
    //glDrawArrays(GL_TRIANGLES, 0, game->colladaMeshNumVertex);
    glDrawElements(GL_TRIANGLES, game->colladaMeshNumVertex, GL_UNSIGNED_INT, 0);
    free(game->matrixArray);
    ///////////////////////////////////////////////////////////////////

}




#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

#include "utility.h"
#include "shader.h"

void LoadShader(
        Shader* shader,
        const char* vertex_path,
        const char* fragment_path)
{
    // buffers to store the content of our shaders...
    char* vertex_buffer = ReadFile(vertex_path);
    char* fragment_buffer = ReadFile(fragment_path);
    if(vertex_buffer == NULL)
    {
        OutputDebugString("ERROR::LOADING::VERTEX::SHADER\n");
        return;
    }
    if(fragment_buffer == NULL)
    {
        OutputDebugString("ERROR::LOADING::FRAGMENT::SHADER\n");
        return;
    }

    // Load OpengGL SHADERS
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    int success;
    char info[512];

    // VERTEXT SHADER...
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_buffer, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info);
        OutputDebugString(info);
    }
    free(vertex_buffer); // free the memory becouse we already compile our shader

    // FRAGMENT SHADER...
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_buffer, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info);
        OutputDebugString(info);
    }
    free(fragment_buffer); // free the memory becouse we already compile our shader

    // Create the SHADER PROGRAM to be use in our engine...
    shader->id = glCreateProgram();
    glAttachShader(shader->id, vertex_shader);
    glAttachShader(shader->id, fragment_shader);
    glLinkProgram(shader->id);
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader->id, 512, NULL, info);
        OutputDebugString(info);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    UseShader(shader);
    shader->projMatLoc  = glGetUniformLocation(shader->id, "Proj");
    shader->viewMatLoc  = glGetUniformLocation(shader->id, "View");
    shader->worldMatLoc = glGetUniformLocation(shader->id, "World");
}

void UseShader(Shader* shader)
{
    glUseProgram(shader->id);
}


void SetShaderMatrix(Matrix m, int matLoc)
{
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, m.m[0]);
}





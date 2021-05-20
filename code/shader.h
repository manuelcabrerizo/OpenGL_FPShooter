#ifndef SHADER_H
#define SHADER_H

#include "math.h"

struct Shader
{
    unsigned int id;
    int worldMatLoc;
    int viewMatLoc;
    int projMatLoc;
};

void LoadShader(Shader* shader,
        const char* vertex_filepath,
        const char* fragment_path);

void UseShader(Shader* shader);
void SetShaderMatrix(Matrix m, int matLoc);

#endif

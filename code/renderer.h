#ifndef RENDERE_H
#define RENDERE_H

#include "shader.h"
#include <vector>

struct RenderUnit
{
    unsigned int vao;
    unsigned int texId;
    int numberMeshes;
    int numberOfVerteces;
    bool haveIndices;
    Shader shader;
    Matrix** model;
    bool** shouldRender;
};

void PushToRender(unsigned int vao,
                  unsigned int texId,
                  int numberMeshes,
                  int numberOfVerteces,
                  bool haveIndices,
                  Shader shader,
                  Matrix* model[],
                  bool* shouldRender[]);
void RenderRendererBuffer();
void ClearRendererBuffer();

#endif

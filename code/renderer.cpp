#include "renderer.h"

#include <windows.h>
#include <glad/glad.h>

std::vector<RenderUnit> renderBuffer;

void PushToRender(unsigned int vao,
                  unsigned int texId,
                  int numberMeshes,
                  int numberOfVerteces,
                  bool haveIndices,
                  Shader shader,
                  Matrix* model[])
{
    RenderUnit renderUnit;
    renderUnit.vao = vao;
    renderUnit.texId = texId;
    renderUnit.numberMeshes = numberMeshes;
    renderUnit.numberOfVerteces = numberOfVerteces;
    renderUnit.haveIndices = haveIndices;
    renderUnit.shader = shader;
    renderUnit.model = model;
    renderBuffer.push_back(renderUnit);
}

void RenderRendererBuffer()
{
    for(int i = 0; i < renderBuffer.size(); i++)
    {
        RenderUnit* renderUnit = &renderBuffer[i];
        UseShader(&renderUnit->shader);
        glBindVertexArray(renderUnit->vao); 
        glBindTexture(GL_TEXTURE_2D, renderUnit->texId);
        for(int j = 0; j < renderUnit->numberMeshes; j++)
        {
            SetShaderMatrix(*renderUnit->model[j], renderUnit->shader.worldMatLoc);
            if(renderUnit->haveIndices)
            { 
                glDrawElements(GL_TRIANGLES, renderUnit->numberOfVerteces, GL_UNSIGNED_INT, 0);
            }
            else
            {
                glDrawArrays(GL_TRIANGLES, 0, renderUnit->numberOfVerteces); 
            }
        }
    }
}

void ClearRendererBuffer()
{
    renderBuffer.clear();
}

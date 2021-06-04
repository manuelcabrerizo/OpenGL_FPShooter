#include "animatedModel.h"
// temps...
#include <stdio.h>
#include <windows.h>

void CalcInverseBindTransform(Joint* joint, Matrix parentBindTransform)
{
    Matrix bindTransform = parentBindTransform * joint->localBindTransform;
    joint->inverseBindTransform = get_inverse_matrix(bindTransform);
    for(int i = 0; i < joint->children.size(); i++)
    {
        CalcInverseBindTransform(&joint->children[i], bindTransform);
    }
}


void ShowJointStruct(Joint* joit)
{
    char buffer[255];
    sprintf(buffer, "%s\n", joit->name.c_str());
    OutputDebugString(buffer);
    sprintf(buffer, "Index: %d\n", joit->index);
    OutputDebugString(buffer);
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            sprintf(buffer, "%f ", joit->localBindTransform.m[y][x]);
            OutputDebugString(buffer);
        }
        OutputDebugString("\n");
    }
    OutputDebugString("inversMatrix\n");
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            sprintf(buffer, "%f ", joit->inverseBindTransform.m[y][x]);
            OutputDebugString(buffer);
        }
        OutputDebugString("\n");
    }
    OutputDebugString("\n");
    for(int i = 0; i < joit->children.size(); i++)
    {
         ShowJointStruct(&joit->children[i]);
    }   
}

void InitAnimatedModel(AnimatedModel* animatedModel,
                       unsigned int vao,
                       unsigned int textId)
{
    animatedModel->vao = vao;
    animatedModel->textId = textId;
    CalcInverseBindTransform(&animatedModel->rootJoint, get_identity_matrix());
}

void AddJointsToArray(Joint* headJoint, Matrix* jointMatrices)
{
    jointMatrices[headJoint->index] =  matrix_transpose(get_rotation_x_matrix(to_radiant(90.0f)) * headJoint->animatedTransform);
    for(int i = 0; i < headJoint->children.size(); i++)
    {
        AddJointsToArray(&headJoint->children[i], jointMatrices);
    }
}

// Esta fuincion ALLOCA MEMORIA remover luego de usar!!!...
float* GetJointTransforms(AnimatedModel* animatedModel)
{
    Matrix* jointMatrices = (Matrix*)malloc(animatedModel->jointCount * sizeof(Matrix));
    AddJointsToArray(&animatedModel->rootJoint, jointMatrices); 
    float* poseMatrices = (float*)malloc(256 * sizeof(float)); 
    for(int j = 0; j < 16; j++)
    {
        for(int y = 0; y < 4; y++)
        {
            for(int x = 0; x < 4; x++)
            {
                poseMatrices[(y*4)+x + j*16] = jointMatrices[j].m[y][x];
            }
        }
    }
    free(jointMatrices);
    return poseMatrices;      
}




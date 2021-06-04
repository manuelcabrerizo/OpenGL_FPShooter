#include "animation.h"
#include "line.h"

#include <stdio.h>
#include <windows.h>

float* GetAnimatedModelPose(Animation* animation, int index)
{
    float* poseMatrices = (float*)malloc(256 * sizeof(float)); 
    KeyFrame* actualFrame = &animation->keyFrames[index];
    for(int j = 0; j < actualFrame->pose.size(); j++)
    {
            for(int y = 0; y < 4; y++)
            {
                for(int x = 0; x < 4; x++)
                {
                    //poseMatrices[(y*4)+x + j*16] = get_identity_matrix().m[y][x];
                    poseMatrices[(y*4)+x + j*16] = GetJointTrasformLocalMatrix(&actualFrame->pose[j]).m[y][x];
                }
            }
        //poseMatrices.push_back(GetJointTrasformLocalMatrix(&actualFrame->pose[j])); 
        //poseMatrices.push_back(get_identity_matrix());        
    }
    //char buffer[100];
    //sprintf(buffer, "%d\n", (int)poseMatrices.size());
    //OutputDebugString(buffer);
    return poseMatrices;
}

void InitJointTransform(JointTransform* jointTransform,
                        Vec3 position,
                        Quaternion rotation)
{
    jointTransform->position = position;
    jointTransform->rotation = rotation;
}

Matrix GetJointTrasformLocalMatrix(JointTransform* jointTransform)
{
    Matrix rotMat = ToRotationMatrix(jointTransform->rotation);
    Matrix posMat = matrix_transpose(get_translation_matrix(jointTransform->position));
    Matrix result = posMat * rotMat;
    result = result;   
    return result;
}

JointTransform InterpolateJointTransform(JointTransform frameA, JointTransform frameB, float progression)
{
    Vec3 pos = Lerp(frameA.position, frameB.position, progression);
    Quaternion rot = QuaternionInterpolate(frameA.rotation, frameB.rotation, progression);
    JointTransform result; 
    InitJointTransform(&result, pos, rot);
    return result; 
}


void ShowAnimationStruct(Animation* animation)
{
    for(int i = 0; i < animation->keyFrames.size(); i++)
    {
        KeyFrame* actualFrame = &animation->keyFrames[i];
        for(int j = 0; j < actualFrame->pose.size(); j++)
        {
            char buffer[100];
            sprintf(buffer, "index: %d\n", j);
            OutputDebugString(buffer);
            for(int y = 0; y < 4; y++)
            {
                for(int x = 0; x < 4; x++)
                {
                    sprintf(buffer, "%f ", GetJointTrasformLocalMatrix(&actualFrame->pose[j]).m[y][x]);
                    OutputDebugString(buffer);
                }
                OutputDebugString("\n");
            }
            OutputDebugString("\n");
        }
    }
}

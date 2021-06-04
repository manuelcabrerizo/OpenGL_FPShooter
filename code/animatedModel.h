#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include "math.h"

#include <string>
#include <vector>


struct Joint
{
    int index;
    std::string name;
    std::vector<Joint> children;
   
    Matrix animatedTransform;

    Matrix localBindTransform;
    Matrix inverseBindTransform;
};

void CalcInverseBindTransform(Joint* joint, Matrix parentBindTransform);
void ShowJointStruct(Joint* joit);


struct AnimatedModel
{
    unsigned int vao;
    unsigned int textId;
    Joint rootJoint;
    int jointCount;
};

void InitAnimatedModel(AnimatedModel* animatedModel,
                       unsigned int vao,
                       unsigned int textId);

void AddJointsToArray(Joint* headJoint, Matrix* jointMatrices);
float* GetJointTransforms(AnimatedModel* animatedModel);


#endif

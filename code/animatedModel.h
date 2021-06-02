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

void InitJoint(Joint* joint, int index, std::string name, Matrix localBindTransform);
void AddJointChild(Joint* joint, Joint children);
void CalcInverseBindTransform(Joint* joint, Matrix parentBindTransform);

struct AnimatedModel
{
    float* vertexBuffer;
    unsigned int vao;
    unsigned int textId;
    std::vector<IVec3> jointIDs;  
    std::vector<Vec3> weights;



    Joint rootJoint;
    int jointCount;
    //Animator animator;
};

void InitAnimatedModel(AnimatedModel* animatedModel,
                       unsigned int vao,
                       unsigned int textId,
                       Joint rootJoint,
                       int jointCount);

void AddJointsToArray(Joint* headJoint, Matrix* jointMatrices);
Matrix* GetJointTransforms(AnimatedModel* animatedModel);


#endif

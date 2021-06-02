#include "animatedModel.h"

void InitJoint(Joint* joint, int index, std::string name, Matrix localBindTransform)
{
    joint->index = index;
    joint->name = name;
    joint->localBindTransform = localBindTransform;
}

void AddJointChild(Joint* joint, Joint children)
{
    joint->children.push_back(children);
}

void CalcInverseBindTransform(Joint* joint, Matrix parentBindTransform)
{
    Matrix bindTransform = parentBindTransform * joint->localBindTransform;
    joint->inverseBindTransform = get_inverse_matrix(bindTransform);
    for(int i = 0; i < joint->children.size(); i++)
    {
        CalcInverseBindTransform(&joint->children[i], bindTransform);
    }
}

void InitAnimatedModel(AnimatedModel* animatedModel,
                       unsigned int vao,
                       unsigned int textId,
                       Joint rootJoint,
                       int jointCount)
{
    animatedModel->vao = vao;
    animatedModel->textId = textId;
    animatedModel->rootJoint = rootJoint;
    animatedModel->jointCount = jointCount;
    //TODO(Manuto): initialize Animator
    CalcInverseBindTransform(&animatedModel->rootJoint, get_identity_matrix());
}

void AddJointsToArray(Joint* headJoint, Matrix* jointMatrices)
{
    jointMatrices[headJoint->index] = headJoint->animatedTransform;
    for(int i = 0; i < headJoint->children.size(); i++)
    {
        AddJointsToArray(&headJoint->children[i], jointMatrices);
    }
}

// Esta fuincion ALLOCA MEMORIA remover luego de usar!!!...
Matrix* GetJointTransforms(AnimatedModel* animatedModel)
{
    Matrix* jointMatrices = (Matrix*)malloc(animatedModel->jointCount * sizeof(Matrix));
    AddJointsToArray(&animatedModel->rootJoint, jointMatrices);
    return jointMatrices;      
}

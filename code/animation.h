#ifndef ANIMATION_H
#define ANIMATION_H

#include "math.h"

#include <vector>
#include <map>
#include <string>

struct JointTransform
{
    Vec3 position;
    Quaternion rotation;
};

void InitJointTransform(JointTransform* jointTransform,
                        Vec3 position,
                        Quaternion rotation);
Matrix GetJointTrasformLocalMatrix(JointTransform* jointTransform);
JointTransform InterpolateJointTransform(JointTransform frameA,
                                         JointTransform frameB,
                                         float progression);
struct KeyFrame
{
    float timeStamp;
    std::map<int, JointTransform> pose;

};

struct Animation
{
    float length;
    std::vector<KeyFrame> keyFrames;
};

void ShowAnimationStruct(Animation* animation);
float* GetAnimatedModelPose(Animation* animation, int index);

#endif

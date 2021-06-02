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
    std::map<std::string, JointTransform> pose;

};

struct Animation
{
    float length;
    std::vector<KeyFrame> keyFrames;
};

void InitAnimation(Animation* animation,
                   float length,
                   std::vector<KeyFrame> keyFrames);

#endif

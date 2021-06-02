#include "animation.h"
#include "line.h"

void InitAnimation(Animation* animation,
                   float length,
                   std::vector<KeyFrame> keyFrames)
{
    animation->length = length;
    for(int i = 0; i < keyFrames.size(); i++)
    {
        animation->keyFrames.push_back(keyFrames[i]);
    }
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
    Matrix rotMat = GetQuaternionRotationMatrix(jointTransform->rotation);
    Matrix posMat = get_translation_matrix(jointTransform->position);
    return rotMat * posMat;
}

JointTransform InterpolateJointTransform(JointTransform frameA, JointTransform frameB, float progression)
{
    Vec3 pos = Lerp(frameA.position, frameB.position, progression);
    Quaternion rot = QuaternionInterpolate(frameA.rotation, frameB.rotation, progression);
    JointTransform result; 
    InitJointTransform(&result, pos, rot);
    return result; 
}

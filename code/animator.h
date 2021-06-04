#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "animatedModel.h" 
#include "animation.h"

struct Animator
{
    AnimatedModel* entity;
    Animation* currentAnimation;
    float animationTime = 0.0f;
};

void InitAnimator(Animator* animator,
                  AnimatedModel* entity);
void DoAnimation(Animator* animator, Animation* animation);
void AnimatorUpdate(Animator* animator, float deltaTime);
std::map<int, Matrix> CalculateCurrentAnimationPose(Animator* animator);
void GetPreviousAndNextKeyFrames(Animator* animator, KeyFrame* frames);
float CalculateProgression(Animator* animator, KeyFrame frame0, KeyFrame frame1);
std::map<int, Matrix> InterpolatePoses(KeyFrame frame0, KeyFrame frame1, float progression);
void ApplayPoseToJoints(std::map<int, Matrix> currentPos, Joint* joint, Matrix parentTransform);

#endif

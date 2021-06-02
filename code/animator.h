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

#endif

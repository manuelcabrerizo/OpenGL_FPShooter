#include "animator.h"

void InitAnimator(Animator* animator,
                  AnimatedModel* entity)
{
    animator->entity = entity;
}

void DoAnimation(Animator* animator, Animation* animation)
{
    animator->animationTime = 0.0f;
    animator->currentAnimation = animation;
}

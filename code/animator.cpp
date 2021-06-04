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


void AnimatorUpdate(Animator* animator, float deltaTime)
{
    if(animator->currentAnimation == NULL)
    {
        return;
    }
    animator->animationTime += deltaTime;
    if(animator->animationTime > animator->currentAnimation->length)
    {
       animator->animationTime = 0.0f;
    }
    std::map<int, Matrix> currentPos = CalculateCurrentAnimationPose(animator);
    ApplayPoseToJoints(currentPos, &animator->entity->rootJoint, get_identity_matrix());
}

void ApplayPoseToJoints(std::map<int, Matrix> currentPos, Joint* joint, Matrix parentTransform)
{
    Matrix currentLocalTransform = currentPos[joint->index];
    Matrix currentTransform = parentTransform * currentLocalTransform;
    for(int i = 0; i < joint->children.size(); i++)
    {
        ApplayPoseToJoints(currentPos, &joint->children[i], currentTransform);
    }
    currentTransform = currentTransform * joint->inverseBindTransform;
    joint->animatedTransform = currentTransform;
}


std::map<int, Matrix> CalculateCurrentAnimationPose(Animator* animator)
{
    KeyFrame frames[2];
    GetPreviousAndNextKeyFrames(animator, frames);
    float progression = CalculateProgression(animator, frames[0], frames[1]);
    return InterpolatePoses(frames[0], frames[1], progression);
}

float CalculateProgression(Animator* animator, KeyFrame frame0, KeyFrame frame1)
{
    float totalTime = frame1.timeStamp - frame0.timeStamp;
    float currentTime = animator->animationTime - frame0.timeStamp;
    return currentTime / totalTime;
}

std::map<int, Matrix> InterpolatePoses(KeyFrame frame0, KeyFrame frame1, float progression)
{
    std::map<int, Matrix> currentPose;
    for(int i = 0; i < frame0.pose.size(); i++) 
    {
        JointTransform previousTransform = frame0.pose[i];
        JointTransform nextTransform     = frame1.pose[i];
        JointTransform currentTransform = InterpolateJointTransform(previousTransform, nextTransform, progression);
        currentPose.insert(std::pair<int, Matrix>(i, GetJointTrasformLocalMatrix(&currentTransform)));
    }
    return currentPose;
}

void GetPreviousAndNextKeyFrames(Animator* animator, KeyFrame* frames)
{
    std::vector<KeyFrame> allKeyFrames = animator->currentAnimation->keyFrames;
    KeyFrame previousKeyFrame = allKeyFrames[0];
    KeyFrame nextKeyFrame = allKeyFrames[0];
    for(int i = 1; i < allKeyFrames.size(); i++)
    {
        nextKeyFrame = allKeyFrames[i];
        if(nextKeyFrame.timeStamp > animator->animationTime)
        {
            break;
        }
        previousKeyFrame = allKeyFrames[i];
    }
    frames[0] = previousKeyFrame;
    frames[1] = nextKeyFrame;
}



#include "AnimationManager.h"

PARAMS_WORLD_MANAGER_INIT AnimationManager::wmi;

AnimationManager::AnimationManager() {}
AnimationManager::~AnimationManager() {}

void AnimationManager::InitAnim(Anim anim)
{
    switch (anim)
    {
    case ANIM_WORLD_MANAGER_INIT:
        AnimationManager::wmi = {};
        break;
    
    default:
        break;
    }
}

void AnimationManager::StepAnim(Anim anim) {}

void AnimationManager::RenderAnim(Anim anim, Renderer* rr) {}

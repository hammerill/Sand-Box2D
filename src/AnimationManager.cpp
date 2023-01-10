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

void AnimationManager::StepAnim(Anim anim)
{
    switch (anim)
    {
    case ANIM_WORLD_MANAGER_INIT:
        if (AnimationManager::wmi.frames < 250)
        {
            switch (AnimationManager::wmi.frames)
            {
            case 60:
                AnimationManager::wmi.goal_pos = 1;
                AnimationManager::wmi.goal_bg_opaque = 0.75;
                break;
            
            default:
                break;
            }
            AnimationManager::wmi.frames++;
        }
        break;
    
    default:
        break;
    }
}

void AnimationManager::RenderAnim(Anim anim, Renderer* rr)
{
    switch (anim)
    {
    case ANIM_WORLD_MANAGER_INIT:
        if (AnimationManager::wmi.frames < 250)
        {
            
        }
        break;
    
    default:
        break;
    }
}

float AnimationManager::TransitionEaseInOutSine(float linear)
{
    return -(cos(M_PI * linear) - 1) / 2;
}

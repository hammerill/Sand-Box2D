#include "AnimationManager.h"

PARAMS_WORLD_MANAGER_INIT AnimationManager::wmi;
PARAMS_FADE AnimationManager::fade;

AnimationManager::AnimationManager() {}
AnimationManager::~AnimationManager() {}

void AnimationManager::InitAnim(Anim anim)
{
    switch (anim)
    {
    case ANIM_WORLD_MANAGER_INIT:
        AnimationManager::wmi = {};

        AnimationManager::wmi.transition_text_opaque.SetTransition(0, 1, 10, 50);

        AnimationManager::wmi.transition_pos.SetTransition(0, 0.75, 90, 40);
        AnimationManager::wmi.transition_bg_opaque.SetTransition(1, 0.50, 90, 40);
        break;
    case ANIM_FADE_IN:
        AnimationManager::fade = {};
        AnimationManager::fade.transition_opaque.SetTransition(1, 0, 0, 100);
        break;
    case ANIM_FADE_OUT:
        AnimationManager::fade = {};
        AnimationManager::fade.transition_opaque.SetTransition(0, 1, 0, 100);
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
        if (AnimationManager::wmi.frames < 300)
        {
            switch (AnimationManager::wmi.frames)
            {
            case 250:
                AnimationManager::wmi.transition_pos.SetTransition(0.75, 2, 250, 40);
                break;
            
            default:
                break;
            }

            AnimationManager::wmi.transition_text_opaque.ApplyTransition(
                AnimationManager::wmi.text_opaque, AnimationManager::wmi.frames, &TransitionEaseInOutSine);
            AnimationManager::wmi.transition_pos.ApplyTransition(
                AnimationManager::wmi.pos, AnimationManager::wmi.frames, &TransitionEaseInOutSine);
            AnimationManager::wmi.transition_bg_opaque.ApplyTransition(
                AnimationManager::wmi.bg_opaque, AnimationManager::wmi.frames, &TransitionEaseInOutSine);

            AnimationManager::wmi.frames++;
        }
        break;
    case ANIM_FADE_IN: case ANIM_FADE_OUT:
        if (AnimationManager::fade.frames <= 100)
        {
            AnimationManager::fade.transition_opaque.ApplyTransition(
                AnimationManager::fade.opaque, AnimationManager::fade.frames, &TransitionEaseInOutSine);

            AnimationManager::wmi.frames++;
        }
    
    default:
        break;
    }
}

void AnimationManager::RenderAnim(Anim anim, Renderer* rr)
{
    switch (anim)
    {
    case ANIM_WORLD_MANAGER_INIT:
        if (AnimationManager::wmi.frames < 300)
        {
            int pospx = rr->GetWindowParams().height * AnimationManager::wmi.pos;

            if (AnimationManager::wmi.pos < 1)
            {
                SDL_Rect rect {0, pospx, rr->GetWindowParams().width, rr->GetWindowParams().height - pospx};

                SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, AnimationManager::wmi.bg_opaque * 0xFF);

                SDL_RenderFillRect(rr->GetRenderer(), &rect);
            }

            uint8_t text_color = AnimationManager::wmi.text_opaque * 0xFF;

            rr->RenderText(
                AnimationManager::wmi.levelname, rr->GetWindowParams().width / 2,
                (rr->GetWindowParams().height + pospx) / 2, AnimationManager::wmi.text_scale, true,
                text_color, text_color, text_color);
        }
        break;
    case ANIM_FADE_IN: case ANIM_FADE_OUT:
        {
            SDL_Rect rect {0, 0, rr->GetWindowParams().width, rr->GetWindowParams().height};

            SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, AnimationManager::fade.opaque * 0xFF);

            SDL_RenderFillRect(rr->GetRenderer(), &rect);
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

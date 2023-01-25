#include "Animations.h"

PARAMS_WORLD_MANAGER_INIT AnimationManager::wmi;
PARAMS_FADE AnimationManager::fade;
PARAMS_VITA_INIT AnimationManager::vita;

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
        AnimationManager::fade.transition_opaque.SetTransition(1, 0, 0, 60);
        break;
    case ANIM_FADE_OUT:
        AnimationManager::fade = {};
        AnimationManager::fade.transition_opaque.SetTransition(0, 1, 0, 60);
        break;

    case ANIM_VITA_INIT:
        AnimationManager::vita = {};
        AnimationManager::vita.transition_opaque.SetTransition(1, 0, 0, 60);
        break;
    
    default:
        break;
    }
}

bool AnimationManager::StepAnim(Anim anim)
{
    switch (anim)
    {
    case ANIM_WORLD_MANAGER_INIT:
        if (AnimationManager::wmi.frames <= AnimationManager::wmi.frames_max)
        {
            switch (AnimationManager::wmi.frames)
            {
            case 250:
                AnimationManager::wmi.transition_pos.SetTransition(0.75, 2, AnimationManager::wmi.frames_max - 40, 40);
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

            return true;
        }
        return false;

    case ANIM_FADE_IN: case ANIM_FADE_OUT: case ANIM_FADE:
        if (AnimationManager::fade.frames <= AnimationManager::fade.frames_max)
        {
            AnimationManager::fade.transition_opaque.ApplyTransition(
                AnimationManager::fade.opaque, AnimationManager::fade.frames, &TransitionEaseInOutSine);

            AnimationManager::fade.frames++;

            return true;
        }
        return false;

    case ANIM_VITA_INIT:
        if (AnimationManager::vita.frames <= AnimationManager::vita.frames_max)
        {
            AnimationManager::vita.transition_opaque.ApplyTransition(
                AnimationManager::vita.opaque, AnimationManager::vita.frames, &TransitionEaseInOutSine);

            AnimationManager::vita.frames++;

            return true;
        }
        return false;

    default:
        return false;
    }
}

void AnimationManager::RenderAnim(Anim anim, Renderer* rr)
{
    switch (anim)
    {
    case ANIM_WORLD_MANAGER_INIT:
        if (AnimationManager::wmi.frames <= AnimationManager::wmi.frames_max)
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
                AnimationManager::wmi.level_name.c_str(), rr->GetWindowParams().width / 2,
                (rr->GetWindowParams().height + pospx) / 2, AnimationManager::wmi.text_scale, true, Translations::GetJp(),
                text_color, text_color, text_color);
        }
        break;

    case ANIM_FADE_IN: case ANIM_FADE_OUT: case ANIM_FADE:
        {
            SDL_Rect rect {0, 0, rr->GetWindowParams().width, rr->GetWindowParams().height};

            SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, AnimationManager::fade.opaque * 0xFF);

            SDL_RenderFillRect(rr->GetRenderer(), &rect);
        }
        break;

    case ANIM_VITA_INIT:
        {
            if (AnimationManager::vita.pic == nullptr)
                AnimationManager::vita.pic = SDL_CreateTextureFromSurface(rr->GetRenderer(), IMG_Load("sce_sys/pic0.png"));

            SDL_RenderCopyEx(rr->GetRenderer(), AnimationManager::vita.pic, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
            
            SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, (1 - AnimationManager::fade.opaque) * 0xFF);
            
            SDL_Rect rect {0, 0, rr->GetWindowParams().width, rr->GetWindowParams().height};
            SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_NONE);
            SDL_RenderFillRect(rr->GetRenderer(), &rect);
        }
        break;

    default:
        break;
    }
}

void AnimationManager::SetLevelName(std::string level_name)
{
    AnimationManager::wmi.level_name = level_name;
}

float AnimationManager::TransitionEaseInOutSine(float linear)
{
    return -(cos(M_PI * linear) - 1) / 2;
}

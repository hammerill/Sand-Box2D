#pragma once

#include "Renderer.h"
#include <math.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

enum Anim 
{
    ANIM_WORLD_MANAGER_INIT = 0,    // Animation of WorldManager initialization (WMI). Will show levels name.
    ANIM_FADE_IN,                   // Animation of Fade-in, i.e. from black screen to scene.
    ANIM_FADE_OUT                   // Animation of Fade-out, i.e. from scene to black screen.
};

/// @brief Little class to apply transitions.
class Transition
{
private:
    float start_value, end_value;
    int frames_start, frames_length;
public:
    Transition() {}

    /// @param start_value value before applying transition.
    /// @param end_value value after applying transition.
    /// @param frames_start at which frame transition will start.
    /// @param frames_length how much frames transition will last.
    void SetTransition(float start_value, float end_value, int frames_start, int frames_length)
    {
        this->start_value = start_value;
        this->end_value = end_value;
        this->frames_start = frames_start;
        this->frames_length = frames_length;
    }

    /// @param value value to be transitioned.
    /// @param frames frames count on this animation.
    /// @param transition transition function to be applied.
    void ApplyTransition(float& value, int frames, float (*transition)(float))
    {
        if (frames <= this->frames_start)
        {
            value = this->start_value;
            return;
        }
        if (frames > this->frames_start + this->frames_length)
        {
            value = this->end_value;
            return;
        }
        
        value = this->start_value
                +
                (
                    transition
                    (
                        (frames - this->frames_start)
                        /
                        (float)this->frames_length
                    )
                    * 
                    (this->end_value - this->start_value)
                );
    }
};

struct PARAMS_WORLD_MANAGER_INIT
{
    int frames = 0, frames_max = 290;
    const char* levelname = "SAND-BOX2D PRE-RELEASE";
    int text_scale = 4;

    float pos, text_opaque, bg_opaque;
    Transition transition_pos, transition_text_opaque, transition_bg_opaque;
};

struct PARAMS_FADE
{
    int frames = 0, frames_max = 60;

    float opaque;
    Transition transition_opaque;
};

/// Static class for managing game animations.
/// 
/// First you need to init some animation (InitAnim(ANIM_NAME)).
/// Then every frame call StepAnim(ANIM_NAME) and RenderAnim(ANIM_NAME).
/// Look at enum Anim to get all animations list.
class AnimationManager
{
private:
    static PARAMS_WORLD_MANAGER_INIT wmi;
    static PARAMS_FADE fade;
public:
    AnimationManager();
    ~AnimationManager();

    /// @brief Init some animation.
    /// It will reset all the values and start playing animation from beginning.
    static void InitAnim(Anim anim);
    /// @brief Make a logical step of animation.
    /// @return true if animations next frame can be rendered. False if it's the end.
    static bool StepAnim(Anim anim);
    /// @brief Render animation.
    static void RenderAnim(Anim anim, Renderer* rr);

    /// @brief Function used to make transition "Ease-In-Out Sine".
    /// Credits to easings.net
    /// @param linear value of input linear function.
    /// @return transitioned value.
    static float TransitionEaseInOutSine(float linear);
};

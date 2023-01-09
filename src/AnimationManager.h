#pragma once

#include "Renderer.h"

enum Anim 
{
    ANIM_WORLD_MANAGER_INIT = 0     // Animation of WorldManager initialization (WMI). Will show levels name.
};

struct PARAMS_WORLD_MANAGER_INIT
{
    int frames = 0;
    const char* levelname = "DEFAULT LEVEL";

    int text_size = 4;
    float pos = 0, text_opaque = 0, bg_opaque = 1;
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
public:
    AnimationManager();
    ~AnimationManager();

    /// @brief Init some animation.
    /// It will reset all the values and start playing animation from beginning.
    static void InitAnim(Anim anim);
    /// @brief Make a logical step of animation.
    static void StepAnim(Anim anim);
    /// @brief Render animation.
    static void RenderAnim(Anim anim, Renderer* rr);
};

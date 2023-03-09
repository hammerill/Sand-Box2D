#pragma once

#include <box2d/box2d.h>

#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "../System/Renderer.h"
#include "../System/Controls.h"
#include "../System/Translations.h"
#include "../System/Animations.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

class MainMenuPhysics
{
private:
    b2World* world = nullptr;

    b2Body* paddle = nullptr;
    b2Body* box_logo = nullptr;

    b2Vec2 paddle_desired_pos;
    bool paddle_inited;
    float paddle_width, paddle_height;

    bool box_active;

    uint8_t title_factual_alpha, title_desired_alpha;

    float RAD2DEG;

    float GetRandomFloat(float min, float max);

public:
    MainMenuPhysics();
    ~MainMenuPhysics();

    void Init();
    void InitPaddle(float paddle_width);
    
    void Step();
    void RenderTitle(Renderer* rr, SDL_Rect& title_rect);
    void RenderBox(Renderer* rr, int x_offset, int y_offset);
    void RenderPaddle(Renderer* rr, int x_offset, int y_offset, float menu_scale);

    SDL_Rect GetPaddleRect(Renderer* rr, int x_offset, int y_offset, float menu_scale);

    void ActivateBox(Renderer* rr);
    SDL_Rect GetBoxRect(Renderer* rr, int x_offset, int y_offset);

    void SetPaddleDesiredPosition(b2Vec2 pos);
    void SetPaddlePositionPermanently(b2Vec2 pos);

    void SetTitleDesiredAlpha(uint8_t alpha);
    void SetTitleAlphaPermanently(uint8_t alpha);

    void FreeMemory();
};


/// @brief Class that manages main menu.
class MainMenu
{
private:
    std::vector<std::string> menu_items;
    std::vector<SDL_Texture*> menu_items_textures;
    std::vector<SDL_Texture*> menu_black_items_textures;
    size_t hovered_item = 0;

    MainMenuPhysics physics;

    WindowParams old_wparams, now_wparams;

    /// Status that used after unloading MainMenu.
    /// 
    /// If it contains path to the level, this level has to be loaded (player pressed PLAY).
    /// If it's empty, it means that game needs to be closed (player pressed EXIT).
    std::string status = "";
    std::string level_name = "";

    void RenderWhiteText(Renderer* rr, size_t index, int x, int y, float scale);
    void RenderBlackText(Renderer* rr, size_t index, int x, int y, float scale, SDL_Rect paddleRect);
public:
    MainMenu();
    ~MainMenu();

    /// @brief Init menu.
    void Init(Renderer* rr);
    /// @brief Destroy menu.
    void FreeMemory();

    /// @brief Make MainMenu logical step.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    /// @param ctrl keys pressed in this frame.
    /// @param old_ctrl keys pressed in previous frame.
    /// @return true if MainMenu wants to render next frame. False if it's the end and we need to load something else
    /// (also it will look at "status" variable).
    bool Step(Renderer* rr, Controls ctrl, Controls old_ctrl);

    /// @brief Render MainMenu.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    void Render(Renderer* rr);

    /// @brief Get "status" variable. It contains what item player decided to select in menu.
    /// For more info refer to MainMenu::status.
    std::string GetStatus();
    std::string GetLevelName();
};


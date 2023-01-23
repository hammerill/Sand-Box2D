#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Renderer.h"
#include "Controls.h"
#include "Translations.h"
#include "Animations.h"
#include "Settings.h"

class LangSelector
{
private:
    std::string lang_code = "";
public:
    LangSelector();
    ~LangSelector();

    /// @brief Init LangSelector.
    void Init();

    /// @brief Make LangSelector logical step.
    /// @param settings link to settings manager. Used to store and read chosen language param.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    /// @param ctrl keys pressed in this frame.
    /// @param old_ctrl keys pressed in previous frame.
    /// @return true if LangSelector wants to render next frame.
    /// False if it's the end and we know what language user wants (call GetLangCode() to get it).
    bool Step(Settings* settings, Renderer* rr, Controls ctrl, Controls old_ctrl);

    /// @brief Render LangSelector.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    void Render(Renderer* rr);

    /// @brief Get code of the language that user chose.
    std::string GetLangCode();
};

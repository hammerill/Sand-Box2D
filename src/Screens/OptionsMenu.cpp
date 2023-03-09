#include "OptionsMenu.h"

OptionsMenu::OptionsMenu() {}
OptionsMenu::~OptionsMenu() {}

void OptionsMenu::Init(std::string translations_base)
{
    OptionsMenu::translations_base = translations_base;

    OptionsMenu::langs = std::map<std::string, std::string>();
    OptionsMenu::hovered_lang = 0;

    AnimationManager::InitAnim(ANIM_FADE_IN);
    OptionsMenu::fadeout = false;
}

bool OptionsMenu::Step(Settings* settings, Renderer* rr, Controls ctrl, Controls old_ctrl)
{
    if (settings->Get("language").asString() != "0" && !OptionsMenu::fadeout)
    {
        Translations::LoadTranslation(OptionsMenu::translations_base, settings->Get("language").asString());
        return false;
    }

    if (AnimationManager::StepAnim(ANIM_FADE))
        return true;
    else if (fadeout)
        return false;
    
    if (ctrl.MenuEnter())
    {
        rr->GetSounds()->PlaySfx("menu_enter");
        OptionsMenu::fadeout = true;
        AnimationManager::InitAnim(ANIM_FADE_OUT);

        return true;
    }

    if (rr->GetCursor(old_ctrl))
    {
        int langSelectScale = rr->GetHeight() / 200;
        SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", langSelectScale);

        float distanceScale = 1.2;
        
        int menu_h = (OptionsMenu::langs.size()) * textDimensions.h * distanceScale;
        int y_offset = (rr->GetHeight() / 2) - (menu_h / 2);

        size_t i = 0;
        for (auto const& lang : OptionsMenu::langs)
        {
            SDL_Rect hover = rr->GetFont()->GetTextDimensions(lang.second.c_str(), langSelectScale);

            hover.x = (rr->GetWidth() / 2) - (hover.w / 2) - textDimensions.w / 16;
            hover.y = y_offset + textDimensions.h * distanceScale * (int)i - (hover.h / 2) - textDimensions.h / 16;
            hover.w += textDimensions.w / 8;
            hover.h += textDimensions.h / 8;

            SDL_Point temp = ctrl.GetMouse();
            if (SDL_PointInRect(&temp, &hover))
            {
                if (i != OptionsMenu::hovered_lang)
                {
                    rr->GetSounds()->PlaySfx("menu_switch");
                    OptionsMenu::hovered_lang = i;
                }
                if (old_ctrl.MenuMouse() && !ctrl.MenuMouse())
                {
                    rr->GetSounds()->PlaySfx("menu_enter");
                    OptionsMenu::fadeout = true;
                    AnimationManager::InitAnim(ANIM_FADE_OUT);
                }
                return true;
            }

            i++;
        }
    }

    if (ctrl.MenuUp() && !old_ctrl.MenuUp())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        OptionsMenu::hovered_lang = OptionsMenu::hovered_lang == 0
                                    ? OptionsMenu::langs.size() - 1
                                    : OptionsMenu::hovered_lang - 1;
    }
    if (ctrl.MenuDown() && !old_ctrl.MenuDown())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        OptionsMenu::hovered_lang = OptionsMenu::hovered_lang == OptionsMenu::langs.size() - 1
                                    ? 0
                                    : OptionsMenu::hovered_lang + 1;
    }
        
    return true;
}

void OptionsMenu::Render(Renderer* rr)
{
    bool hover_blinker = OptionsMenu::fadeout == true && (int)(rr->GetFrames() / 10) % 2;

    int langSelectScale = rr->GetHeight() / 200;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", langSelectScale);

    float distanceScale = 1.2;
    
    int menu_h = (OptionsMenu::langs.size()) * textDimensions.h * distanceScale;
    int y_offset = (rr->GetHeight() / 2) - (menu_h / 2);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0x10, 0x10, 0x10, 0);
    SDL_RenderClear(rr->GetRenderer());

    size_t i = 0;
    for (auto const& lang : OptionsMenu::langs)
    {
        if (i == OptionsMenu::hovered_lang)
        {
            SDL_Rect hover_bg = rr->GetFont()->GetTextDimensions(lang.second.c_str(), langSelectScale);

            hover_bg.x = (rr->GetWidth() / 2) - (hover_bg.w / 2) - textDimensions.w / 16;
            hover_bg.y = y_offset + textDimensions.h * distanceScale * (int)i - (hover_bg.h / 2) - textDimensions.h / 16;
            hover_bg.w += textDimensions.w / 8;
            hover_bg.h += textDimensions.h / 8;

            SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(rr->GetRenderer(), &hover_bg);
            
            rr->RenderText(
                "▶",
                rr->GetWidth() / 2 - rr->GetFont()->GetTextDimensions(lang.second.c_str(), langSelectScale).w / 2 - textDimensions.w,
                y_offset + textDimensions.h * distanceScale * i,
                langSelectScale,
                true,
                true
            );

            if (!hover_blinker)
                rr->RenderText(
                    lang.second.c_str(),
                    rr->GetWidth() / 2,
                    y_offset + textDimensions.h * distanceScale * i,
                    langSelectScale,
                    true,
                    Translations::GetJp(), 0, 0, 0
                );
        }
        else
            rr->RenderText(
                lang.second.c_str(),
                rr->GetWidth() / 2,
                y_offset + textDimensions.h * distanceScale * i,
                langSelectScale,
                true,
                Translations::GetJp()
            );
        
        i++;
    }

    rr->RenderText(
        OptionsMenu::choose_title.c_str(),
        rr->GetWidth() / 2,
        rr->GetHeight() / 16,
        Translations::GetJp() ? langSelectScale : langSelectScale / 2,
        true, Translations::GetJp()
    );
    rr->RenderText(
        OptionsMenu::settings_reminder.c_str(),
        rr->GetWidth() / 2,
        (rr->GetHeight() / 16) * 15,
        Translations::GetJp() ? langSelectScale : langSelectScale / 2,
        true, Translations::GetJp()
    );
    
    AnimationManager::RenderAnim(ANIM_FADE, rr);
}

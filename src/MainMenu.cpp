#include "MainMenu.h"

MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}

void MainMenu::Init(std::string translations_base)
{
    MainMenu::menu_items = std::vector<std::string>();
    MainMenu::hovered_item = 0;
    MainMenu::status = "";

    MainMenu::lang_selector.Init(translations_base);
    MainMenu::lang_chosen = false;
}

bool MainMenu::Step(Settings* settings, Renderer* rr, Controls ctrl, Controls old_ctrl)
{
    if (!MainMenu::lang_chosen)
    {
        if (MainMenu::lang_selector.Step(settings, rr, ctrl, old_ctrl))
            return true;
        else
        {
            MainMenu::menu_items.push_back(Translations::Load("menu.json/item_play"));
            MainMenu::menu_items.push_back(Translations::Load("menu.json/item_about"));
            MainMenu::menu_items.push_back(Translations::Load("menu.json/item_exit"));

            AnimationManager::InitAnim(ANIM_FADE_IN);

            MainMenu::lang_chosen = true;
        }
    }
    
    if (AnimationManager::StepAnim(ANIM_FADE))
        return true; // If screen still fades we don't care about controls and do early return.
    else
    {
        if (MainMenu::status == "fadeout")
        {
            switch (MainMenu::hovered_item)
            {
            case 0:
                MainMenu::status = "play";
                MainMenu::level_name = "DEFAULT LEVEL";
                break;
            case 1:
                MainMenu::status = "about";
                break;
            
            default:
                MainMenu::status = "";
                break;
            }
            return false; // False signifies that we've reached the end of MainMenu lifecycle.
            // From now on, its `will` will be stored only at "status" variable.
        }
    }

    if (ctrl.MenuEnter() && !old_ctrl.MenuEnter())
    {
        rr->GetSounds()->PlaySfx("menu_enter");
        MainMenu::status = "fadeout";
        AnimationManager::InitAnim(ANIM_FADE_OUT);
        return true;
    }

    if (rr->GetCursor())
    {
        int menuScale = rr->GetWindowParams().height / 200;
        SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

        float distanceScale = 1.2;

        std::vector<int> menuWidths;
        for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
            menuWidths.push_back(rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale).w);

        int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
        int menu_h = (MainMenu::menu_items.size()) * textDimensions.h * distanceScale;

        int x_offset = (rr->GetWindowParams().width / 4) - (menu_w / 2);
        int y_offset = (rr->GetWindowParams().height / 2) - (menu_h / 2);

        for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        {
            SDL_Rect hover = rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale);

            hover.x = x_offset - textDimensions.w / 16;
            hover.y = y_offset + textDimensions.h * distanceScale * (int)i - textDimensions.h / 16;
            hover.w += textDimensions.w / 8;
            hover.h += textDimensions.h / 8;

            SDL_Point temp = ctrl.GetMouse();
            if (SDL_PointInRect(&temp, &hover))
            {
                if (i != MainMenu::hovered_item)
                {
                    rr->GetSounds()->PlaySfx("menu_switch");
                    MainMenu::hovered_item = i;
                }
                if (old_ctrl.IsMoving() && !ctrl.IsMoving()) // Maybe I should set another name for this ctrl.
                {
                    rr->GetSounds()->PlaySfx("menu_enter");
                    MainMenu::status = "fadeout";
                    AnimationManager::InitAnim(ANIM_FADE_OUT);
                }
                return true;
            }
        }
    }
    
    if (ctrl.MenuUp() && !old_ctrl.MenuUp())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        MainMenu::hovered_item = MainMenu::hovered_item == 0
                                    ? MainMenu::menu_items.size() - 1
                                    : MainMenu::hovered_item - 1;
    }
    if (ctrl.MenuDown() && !old_ctrl.MenuDown())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        MainMenu::hovered_item = MainMenu::hovered_item == MainMenu::menu_items.size() - 1
                                    ? 0
                                    : MainMenu::hovered_item + 1;
    }
        
    return true;
}

void MainMenu::Render(Renderer* rr)
{
    if (!MainMenu::lang_chosen)
    {
        MainMenu::lang_selector.Render(rr);
        return;
    }

    bool hover_blinker = MainMenu::status == "fadeout" && (int)(rr->GetFrames() / 10) % 2;

    int menuScale = rr->GetWindowParams().height / 200;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (MainMenu::menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWindowParams().width / 4) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 2) - (menu_h / 2);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(rr->GetRenderer());

    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
    {
        if (i == MainMenu::hovered_item && !hover_blinker)
        {
            SDL_Rect hover_bg = rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale);

            hover_bg.x = x_offset - textDimensions.w / 16;
            hover_bg.y = y_offset + textDimensions.h * distanceScale * (int)i - textDimensions.h / 16;
            hover_bg.w += textDimensions.w / 8;
            hover_bg.h += textDimensions.h / 8;

            SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(rr->GetRenderer(), &hover_bg);

            rr->RenderText(
                MainMenu::menu_items[i].c_str(),
                x_offset,
                y_offset + textDimensions.h * distanceScale * i,
                menuScale,
                false,
                Translations::GetJp(), 0, 0, 0
            );
            rr->RenderText(
                "▶",
                x_offset - textDimensions.w * distanceScale,
                y_offset + textDimensions.h * distanceScale * i,
                menuScale,
                false,
                true
            );
        }
        else
            rr->RenderText(
                MainMenu::menu_items[i].c_str(),
                x_offset,
                y_offset + textDimensions.h * distanceScale * i,
                menuScale,
                false,
                Translations::GetJp()
            );
    }
    
    AnimationManager::RenderAnim(ANIM_FADE, rr);
}

std::string MainMenu::GetStatus()   { return MainMenu::status; }

std::string MainMenu::GetLevelName()    { return MainMenu::level_name; }

#include "MainMenu.h"

MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}

void MainMenu::Init(std::string translations_base)
{
    MainMenu::menu_items = std::vector<std::string>();
    MainMenu::hovered_item = 0;
    MainMenu::status = "";

    Translations::LoadTranslation(translations_base, "en");

    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_play"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_about"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_exit"));

    AnimationManager::InitAnim(ANIM_FADE_IN);
}

bool MainMenu::Step(Renderer* rr, Controls ctrl, Controls old_ctrl)
{
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
                MainMenu::level_name = "DEFAULT LEVELOS";
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
    if (ctrl.MenuEnter() && !old_ctrl.MenuEnter())
    {
        rr->GetSounds()->PlaySfx("menu_enter");
        MainMenu::status = "fadeout";
        AnimationManager::InitAnim(ANIM_FADE_OUT);
    }
        
    return true;
}

void MainMenu::Render(Renderer* rr)
{
    // There was some s***code but I've fixed it so you have to search git commits to see it.
    bool hover_blinker = MainMenu::status == "fadeout" && (int)(rr->GetFrames() / 10) % 2;

    int menuScale = rr->GetWindowParams().height / 100;
    int fontWidth = rr->GetFont()->FontWidth;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (MainMenu::menu_items.size()) * fontWidth * menuScale;

    int x_offset = (rr->GetWindowParams().width / 4) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 2) - (menu_h / 2);

    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(rr->GetRenderer());

    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
    {
        if (i == MainMenu::hovered_item && !hover_blinker)
        {
            SDL_Rect hover_bg = rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale);

            hover_bg.x = x_offset + fontWidth * menuScale - fontWidth;
            hover_bg.y = y_offset + fontWidth * menuScale * (int)i - fontWidth;
            hover_bg.w += fontWidth;
            hover_bg.h += fontWidth;

            SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(rr->GetRenderer(), &hover_bg);

            rr->RenderText(MainMenu::menu_items[i].c_str(), x_offset + fontWidth * menuScale, y_offset + fontWidth * menuScale * i, menuScale, false, 0, 0, 0);
        }
        else
            rr->RenderText(MainMenu::menu_items[i].c_str(), x_offset + fontWidth * menuScale, y_offset + fontWidth * menuScale * i, menuScale);
    }
    
    AnimationManager::RenderAnim(ANIM_FADE, rr);
}

std::string MainMenu::GetStatus()   { return MainMenu::status; }

std::string MainMenu::GetLevelName()    { return MainMenu::level_name; }
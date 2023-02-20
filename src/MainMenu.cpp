#include "MainMenu.h"

SDL_Texture* logo;
SDL_Texture* title;

MainMenu::MainMenu() {}
MainMenu::~MainMenu()
{
    SDL_DestroyTexture(logo);
}

void MainMenu::Init(std::string translations_base)
{
    MainMenu::menu_items = std::vector<std::string>();
    MainMenu::hovered_item = 0;
    MainMenu::status = "";

    MainMenu::lang_selector.Init(translations_base);
    MainMenu::lang_chosen = false;
    
    logo = nullptr;  
    title = nullptr;    
}

SDL_Rect GetItemRect(Renderer* rr, std::vector<std::string> menu_items, size_t item_index)
{
    int menuScale = rr->GetWindowParams().height / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWindowParams().width / 2.5) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 1.5) - (menu_h / 2);

    SDL_Rect rect = rr->GetFont()->GetTextDimensions(menu_items[item_index].c_str(), menuScale);

    rect.x = x_offset - textDimensions.w / 16;
    rect.y = y_offset + textDimensions.h * distanceScale * (int)item_index - textDimensions.h / 16;
    rect.w += textDimensions.w / 8;
    rect.h += textDimensions.h / 8;

    return rect;
}

bool MainMenu::Step(Settings* settings, Renderer* rr, Controls ctrl, Controls old_ctrl)
{
    if (logo == nullptr)
        logo = SDL_CreateTextureFromSurface(rr->GetRenderer(), IMG_Load("./assets/img/logo.png"));
    if (title == nullptr)
        title = SDL_CreateTextureFromSurface(rr->GetRenderer(), IMG_Load("./assets/img/title.png"));

    if (!MainMenu::lang_chosen)
    {
        if (MainMenu::lang_selector.Step(settings, rr, ctrl, old_ctrl))
            return true;
        else
        {
            MainMenu::menu_items.push_back(Translations::Load("menu.json/item_play"));
            MainMenu::menu_items.push_back(Translations::Load("menu.json/item_community"));
            MainMenu::menu_items.push_back(Translations::Load("menu.json/item_level_editor"));
            MainMenu::menu_items.push_back(Translations::Load("menu.json/item_settings"));
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
                MainMenu::status = "community";
                break;
            case 2:
                MainMenu::status = "level_editor";
                break;
            case 3:
                MainMenu::status = "settings";
                break;
            case 4:
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

    if (rr->GetCursor(old_ctrl))
    {
        for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        {
            SDL_Point point = ctrl.GetMouse();
            SDL_Rect rect = GetItemRect(rr, MainMenu::menu_items, i);

            if (SDL_PointInRect(&point, &rect))
            {
                if (i != MainMenu::hovered_item)
                {
                    rr->GetSounds()->PlaySfx("menu_switch");
                    MainMenu::hovered_item = i;
                }
                if (old_ctrl.MousePress() && !ctrl.MousePress())
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

    int menuScale = rr->GetWindowParams().height / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (MainMenu::menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWindowParams().width / 2.5) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 1.5) - (menu_h / 2);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 10, 10, 10, 0);
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

    SDL_Rect logo_rect = {
        rr->GetWindowParams().width / 3 - rr->GetWindowParams().height / 16,
        rr->GetWindowParams().height / 5 - rr->GetWindowParams().height / 16,
        rr->GetWindowParams().height / 8,
        rr->GetWindowParams().height / 8
    };
    SDL_RenderCopyEx(rr->GetRenderer(), logo, NULL, &logo_rect, 0, NULL, SDL_FLIP_NONE);

    SDL_Rect title_rect = {
        rr->GetWindowParams().width / 3 + rr->GetWindowParams().height / 11,
        rr->GetWindowParams().height / 5 - 7 * menuScale,
        78 * menuScale * 2,
        7 * menuScale * 2
    };
    SDL_RenderCopyEx(rr->GetRenderer(), title, NULL, &title_rect, 0, NULL, SDL_FLIP_NONE);

    const char* version_string = "v1.0.0 PRE-RELEASE";
    SDL_Rect version_dimensions = rr->GetFont()->GetTextDimensions(version_string, menuScale / 2);
    rr->RenderText(
        version_string,
        rr->GetWindowParams().width - version_dimensions.w,
        rr->GetWindowParams().height - version_dimensions.h,
        menuScale / 2,
        false, false,
        0x40, 0x40, 0x40
    );

    const char* warning_string = "MOST OF THE BUTTONS DON'T WORK YET";
    SDL_Rect warning_dimensions = rr->GetFont()->GetTextDimensions(warning_string, menuScale / 2);
    rr->RenderText(
        warning_string,
        0,
        rr->GetWindowParams().height - warning_dimensions.h,
        menuScale / 2,
        false, false,
        0x40, 0x40, 0x40
    );
    
    AnimationManager::RenderAnim(ANIM_FADE, rr);
}

std::string MainMenu::GetStatus()   { return MainMenu::status; }

std::string MainMenu::GetLevelName()    { return MainMenu::level_name; }

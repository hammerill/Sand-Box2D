#include "MainMenu.h"

MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}

void MainMenu::Init(std::string translations_base)
{
    MainMenu::menu_items = std::vector<std::string>();
    MainMenu::hovered_item = 0;

    Translations::LoadTranslation(translations_base, "en");

    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_play"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_about"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_exit"));
}

bool MainMenu::Step(Controls ctrl, Controls old_ctrl)
{
    if (ctrl.MenuUp() && !old_ctrl.MenuUp())
    {
        MainMenu::hovered_item = MainMenu::hovered_item == 0
                                    ? MainMenu::menu_items.size() - 1
                                    : MainMenu::hovered_item - 1;
    }
    if (ctrl.MenuDown() && !old_ctrl.MenuDown())
    {
        MainMenu::hovered_item = MainMenu::hovered_item == MainMenu::menu_items.size() - 1
                                    ? 0
                                    : MainMenu::hovered_item + 1;
    }
    
    return true;
}

void MainMenu::Render(Renderer* rr)
{
    float menuScale = 6;
    int fontWidth = rr->GetFont()->FontWidth;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        menuWidths.push_back((MainMenu::menu_items[i].size()) * fontWidth * menuScale);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (MainMenu::menu_items.size()) * fontWidth * menuScale;

    int x_offset = (rr->GetWindowParams().width / 4) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 2) - (menu_h / 2);

    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(rr->GetRenderer());

    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
    {
        if (i == MainMenu::hovered_item)
        {
            SDL_Rect hover_bg {
                x_offset + fontWidth * menuScale,
                y_offset + fontWidth * menuScale * i,
                MainMenu::menu_items[i].size() * fontWidth * menuScale,
                fontWidth * menuScale
            };

            SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(rr->GetRenderer(), &hover_bg);

            SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, 0);
            rr->RenderText(MainMenu::menu_items[i].c_str(), x_offset + fontWidth * menuScale, y_offset + fontWidth * menuScale * i, menuScale, false, 0, 0, 0);
        }
        
        rr->RenderText(MainMenu::menu_items[i].c_str(), x_offset + fontWidth * menuScale, y_offset + fontWidth * menuScale * i, menuScale);
    }
}

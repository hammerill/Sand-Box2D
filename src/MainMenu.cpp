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

}

void MainMenu::Render(Renderer* rr)
{

}

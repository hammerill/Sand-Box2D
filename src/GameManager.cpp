#include "GameManager.h"

GameManager::GameManager(const char* path_to_settings, const char* path_to_def_settings)
{
    if (std::string(path_to_settings).find_last_of("\\/") != std::string::npos)
    {
        Files::MakeDirs(std::string(path_to_settings).substr(0, std::string(path_to_settings).find_last_of("\\/")));
    }

    GameManager::settings = Settings(path_to_settings, path_to_def_settings);

    GameManager::rr = new Renderer();
    GameManager::rr->InitVideo(
        GameManager::fullscreen,
        GameManager::settings.Get("path_to_font").asString() == "" ? nullptr : 
            GameManager::settings.Get("path_to_font").asString().c_str(),
        GameManager::settings.Get("path_to_icon").asString() == "" ? nullptr : 
            GameManager::settings.Get("path_to_icon").asString().c_str()
    );
    
    GameManager::world_manager = new WorldManager(
        GameManager::settings.Get("path_to_def_texture").asString(),
        GameManager::settings.Get("physics_quality").asInt(),
        GameManager::settings.Get("moving_inertia_frames").asInt()
    );

    Level level;
    level.LoadFile(
        GameManager::settings.Get("path_to_def_level_base").asString(), 
        GameManager::settings.Get("path_to_def_level").asString()
    );

    GameManager::world_manager->LoadLevel(level, GameManager::rr);

    GameManager::main_menu = MainMenu();
    GameManager::main_menu.Init(GameManager::settings.Get("path_to_translations").asString());
}

bool GameManager::Step()
{
    if (GameManager::settings.Get("speed_correction").asBool())
        GameManager::b = GameManager::a;

    GameManager::old_ctrl = GameManager::ctrl;
    GameManager::ctrl.Check();

    if (ctrl.Exit())
        return false;
    
    if (ctrl.Fullscreen() && !old_ctrl.Fullscreen())
    {
        auto cur_mode = GameManager::rr->GetWindowParams().mode;
        GameManager::rr->ChangeRes(cur_mode != WINDOWED ? windowed : fullscreen);
    }

    // STEPS
    world_manager->Step(GameManager::rr, GameManager::ctrl, GameManager::old_ctrl);
    // main_menu.Step(GameManager::ctrl, GameManager::old_ctrl);
    ////////

    return true;
}

void GameManager::Render()
{
    // RENDER
    world_manager->Render(GameManager::rr, GameManager::ctrl);
    // main_menu.Render(GameManager::rr);
    /////////

    SDL_RenderPresent(GameManager::rr->GetRenderer());
}

void GameManager::Cycle()
{
    bool isRunning = true;

    // Network::SetRepo(GameManager::settings.Get("url_levels").asString());
    // Network::DownloadFile(GameManager::settings.Get("path_to_levels").asString(), "index.json");
    // Network::DownloadFile(GameManager::settings.Get("path_to_levels").asString(), "default_level/default_level.json");

    if (GameManager::settings.Get("speed_correction").asBool())
    {
        while (isRunning)
        {
            GameManager::a = SDL_GetTicks();
            GameManager::delta = GameManager::a - GameManager::b;

            if (GameManager::delta > 1000/60.0)
            {
                isRunning = GameManager::Step();
                GameManager::Render();
            }
        }
    }
    else
    {
        while (isRunning)
        {
            isRunning = GameManager::Step();
            GameManager::Render();
        }
    }
}

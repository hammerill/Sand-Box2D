#include "GameManager.h"

GameManager::GameManager(const char* path_to_settings, const char* path_to_def_settings)
{
    if (std::string(path_to_settings).find_last_of("\\/") != std::string::npos)
    {
        Files::MakeDirs(std::string(path_to_settings).substr(0, std::string(path_to_settings).find_last_of("\\/")));
    }

    GameManager::settings = Settings(path_to_settings, path_to_def_settings);

    GameManager::renderer = new Renderer();
    GameManager::renderer->InitVideo(
        fullscreen,
        GameManager::settings.Get("path_to_icon").asString() == "" ? nullptr : 
        GameManager::settings.Get("path_to_icon").asString().c_str()
    );
    
    GameManager::world_manager = new WorldManager(
        GameManager::settings.Get("physics_quality").asInt(),
        GameManager::settings.Get("moving_inertia_frames").asInt()
    );

    Level level;
    level.LoadFile(
        GameManager::settings.Get("path_to_def_level_base").asString(), 
        GameManager::settings.Get("path_to_def_level").asString(),
        GameManager::renderer->GetRenderer()
    );

    GameManager::world_manager->LoadLevel(level, GameManager::renderer);
}

bool GameManager::Step()
{
    if (GameManager::settings.Get("speed_correction").asBool())
        GameManager::b = GameManager::a;

    GameManager::old_ctrl = GameManager::ctrl;
    GameManager::ctrl.Check();

    if (ctrl.GetExit())
        return false;
    
    if (ctrl.GetFullscreen() && !old_ctrl.GetFullscreen())
    {
        auto cur_mode = GameManager::renderer->GetWindowMode();
        GameManager::renderer->ChangeRes(cur_mode != WINDOWED ? windowed : fullscreen);
    }

    // STEPS
    world_manager->Step(GameManager::renderer, GameManager::ctrl, GameManager::old_ctrl);
    ////////

    return true;
}

void GameManager::Render()
{
    // RENDER
    world_manager->Render(GameManager::renderer, GameManager::ctrl);
    /////////

    SDL_RenderPresent(GameManager::renderer->GetRenderer());
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

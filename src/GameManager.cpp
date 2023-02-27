#include "GameManager.h"

#ifdef Vita
bool vita_inited_video = false;
#else
bool vita_inited_video = true;
#endif

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
        GameManager::settings.Get("path_to_sfx_base").asString() == "" ? nullptr : 
            GameManager::settings.Get("path_to_sfx_base").asString().c_str(),

        GameManager::settings.Get("path_to_font").asString() == "" ? nullptr : 
            GameManager::settings.Get("path_to_font").asString().c_str(),

        GameManager::settings.Get("path_to_font_jp").asString() == "" ? nullptr : 
            GameManager::settings.Get("path_to_font_jp").asString().c_str(),

        GameManager::settings.Get("path_to_icon").asString() == "" ? nullptr : 
            GameManager::settings.Get("path_to_icon").asString().c_str()
    );
    
    GameManager::world_manager = new WorldManager(
        GameManager::settings.Get("path_to_def_texture").asString(),
        GameManager::settings.Get("physics_quality").asInt(),
        GameManager::settings.Get("moving_inertia_frames").asInt()
    );

    GameManager::main_menu = MainMenu();
    GameManager::main_menu.Init();

    GameManager::lang_selector = LangSelector();
    GameManager::lang_selector.Init(GameManager::settings.Get("path_to_translations").asString());
}

const int mouse_frames_duration = 60;

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

#ifndef Vita
    if ((ctrl.GetMouse().x != old_ctrl.GetMouse().x) || (ctrl.GetMouse().y != old_ctrl.GetMouse().y) || (ctrl.MousePress()))
        GameManager::mouse_last_frame_move = GameManager::rr->GetFrames();
    
    GameManager::rr->SetCursor(!(GameManager::rr->GetFrames() > GameManager::mouse_last_frame_move + mouse_frames_duration));
#else
    if (!vita_inited_video)
    {
        if (AnimationManager::StepAnim(ANIM_VITA_INIT))
            return true;
        else
            vita_inited_video = true;
    }
#endif

    // STEPS
    bool key;
    do
    {
        key = false;
        switch (current_visual)
        {
        case MAIN_MENU_VISUAL:
            if (!GameManager::main_menu.Step(GameManager::rr, GameManager::ctrl, GameManager::old_ctrl))
            {
                if (GameManager::main_menu.GetStatus() == "play")
                {
                    Level level;
                    level.LoadFile(
                        GameManager::settings.Get("path_to_def_level_base").asString(), 
                        GameManager::settings.Get("path_to_def_level").asString()
                    );
                    GameManager::world_manager->LoadLevel(level, GameManager::rr);

                    AnimationManager::SetLevelName(GameManager::main_menu.GetLevelName());
                    
                    key = true;
                    current_visual = WORLD_MANAGER_VISUAL;
                }
                else if (GameManager::main_menu.GetStatus() == "settings")
                {
                    GameManager::settings.Clear();
                    
                    GameManager::lang_selector.Init(GameManager::settings.Get("path_to_translations").asString());

                    key = true;
                    current_visual = LANG_SELECTOR_VISUAL;
                }
                else
                    return false;
            }
            break;
        
        case WORLD_MANAGER_VISUAL:
            if (!GameManager::world_manager->Step(GameManager::rr, GameManager::ctrl, GameManager::old_ctrl))
            {
                GameManager::world_manager->FreeMemory();
                GameManager::main_menu.Init();

                key = true;
                current_visual = MAIN_MENU_VISUAL;
            }
            break;
        
        case LANG_SELECTOR_VISUAL:
            if (!GameManager::lang_selector.Step(&(GameManager::settings), GameManager::rr, GameManager::ctrl, GameManager::old_ctrl))
            {
                GameManager::main_menu.Init();

                key = true;
                current_visual = MAIN_MENU_VISUAL;
            }
            break;
        
        default:
            break;
        }
    } while (key);
    ////////

    return true;
}

void GameManager::Render()
{
    if (!vita_inited_video)
    {
        AnimationManager::RenderAnim(ANIM_VITA_INIT, rr);
        SDL_RenderPresent(GameManager::rr->GetRenderer());
        return;
    }

    // RENDER
    switch (current_visual)
    {
    case MAIN_MENU_VISUAL:
        GameManager::main_menu.Render(GameManager::rr);
        break;
    
    case WORLD_MANAGER_VISUAL:
        GameManager::world_manager->Render(GameManager::rr, GameManager::ctrl);
        break;
    
    case LANG_SELECTOR_VISUAL:
        GameManager::lang_selector.Render(GameManager::rr);
        break;
    
    default:
        break;
    }
    /////////

    GameManager::rr->AddFrame();
    SDL_RenderPresent(GameManager::rr->GetRenderer());
}

void GameManager::Cycle()
{
    bool isRunning = true;

    current_visual = LANG_SELECTOR_VISUAL;

    // Network::SetRepo(GameManager::settings.Get("url_levels").asString());
    // Network::DownloadFile(GameManager::settings.Get("path_to_levels").asString(), "index.json");
    // Network::DownloadFile(GameManager::settings.Get("path_to_levels").asString(), "default_level/default_level.json");

    if (!vita_inited_video)
        AnimationManager::InitAnim(ANIM_VITA_INIT);

    if (GameManager::settings.Get("speed_correction").asBool())
    {
        while (isRunning)
        {
            GameManager::a = SDL_GetTicks64();
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

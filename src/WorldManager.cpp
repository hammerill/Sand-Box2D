#include "WorldManager.h"

// #include "Settings.h"
// Settings settings = Settings("./settings.json", "./assets/default_settings.json");

// GONNA BE DEPRECATED SOON
Controls ctrl, old_ctrl;

WindowParams windowed = {WINDOWED, 960, 544}, fullscreen = WindowParams();
///////////////////////////

WorldManager::WorldManager(const char* path_to_font, bool fpsCorrection, const char* path_to_icon, float move_speed, float zoom_speed)
{
    WorldManager::speedCorrection = fpsCorrection;

    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;

    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    WorldManager::world = new b2World(gravity);

    WorldManager::objects = std::vector<BasePObj*>();

    if (path_to_font != nullptr)
        Font::LoadFont(path_to_font);
    
    WorldManager::renderer = Renderer();
    WorldManager::renderer.InitVideo(fullscreen, path_to_icon);
}
WorldManager::~WorldManager()
{
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    {
        WorldManager::DeleteObject(i);
    }

    delete[] WorldManager::world;
}

void WorldManager::AddObject(BasePObj* obj) { WorldManager::order.push_back(obj); }
void WorldManager::DeleteObject(int index)
{
    delete WorldManager::objects[index]; // Call a destructor.
    WorldManager::objects.erase(WorldManager::objects.begin() + index); // Remove from vector.
}

bool WorldManager::Step()
{
    if (WorldManager::speedCorrection)
        WorldManager::b = WorldManager::a;

    old_ctrl = ctrl;
    ctrl.Check();

    if (ctrl.GetExit())
    {
        return false;
    }

    if (ctrl.GetFullscreen() && !old_ctrl.GetFullscreen())
    {
        auto cur_mode = WorldManager::renderer.GetWindowMode();
        WorldManager::renderer.ChangeRes(cur_mode != WINDOWED ? windowed : fullscreen);
    }

    if (ctrl.GetDebug() && !old_ctrl.GetDebug())
    {
        WorldManager::isDebug = !WorldManager::isDebug;
    }        

    WorldManager::y_offset += ctrl.GetMoveUp() * WorldManager::move_speed;
    WorldManager::x_offset -= ctrl.GetMoveRight() * WorldManager::move_speed;
    WorldManager::y_offset -= ctrl.GetMoveDown() * WorldManager::move_speed;
    WorldManager::x_offset += ctrl.GetMoveLeft() * WorldManager::move_speed;

    if (ctrl.GetIsMoving())
    {
        WorldManager::x_offset += ctrl.GetDeltaX();
        WorldManager::y_offset += ctrl.GetDeltaY();
    }

    if (ctrl.GetIsPinching() && ((WorldManager::zoom + ctrl.GetDeltaPinch()) > 10))
    {
        CorrectOffset(  ctrl.GetMouse(),
                        ctrl.GetDeltaPinch());
        WorldManager::zoom += ctrl.GetDeltaPinch();
    }

    SDL_Point scr_center = {WorldManager::renderer.GetWidth() / 2, WorldManager::renderer.GetHeight() / 2};

    if (WorldManager::zoom <= 1)
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        WorldManager::zoom - 1);
        WorldManager::zoom = 1;
    }
    else
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        ctrl.GetZoomOut() * WorldManager::zoom_speed * -1 * WorldManager::zoom);
        WorldManager::zoom -= ctrl.GetZoomOut() * WorldManager::zoom_speed * WorldManager::zoom;
    }

    if (WorldManager::zoom >= 1000)
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        1000 - WorldManager::zoom);
        WorldManager::zoom = 1000;
    }
    else
    {
        CorrectOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        ctrl.GetZoomIn() * WorldManager::zoom_speed * WorldManager::zoom);
        WorldManager::zoom += ctrl.GetZoomIn() * WorldManager::zoom_speed * WorldManager::zoom;
    }

    WorldManager::world->Step(1.0f / 60.0f, 12*4, 4*4);

    if (ctrl.GetReset())
    {
        for (size_t i = 0; i < WorldManager::objects.size(); i++)
        {
            WorldManager::objects[i]->Reset();
        }
    }

    // CYCLES
    auto cycles = WorldManager::level.GetCycles();
    for (size_t i = 0; i < WorldManager::cyclesDelays.size(); i++)
    {
        if (--(WorldManager::cyclesDelays[i]) <= 0)
        {
            auto cycle = cycles[i];

            WorldManager::cyclesDelays[i] = cycle.delay;

            for (size_t j = 0; j < cycle.objects.size(); j++)
            {
                WorldManager::AddObject(cycle.objects[j]);
            }
        }
    }    
    /////////

    
    // LATER IT WILL BE CONSIDERED DEPRECATED AND DESTROYED
    if (ctrl.GetReloadLevel() && !old_ctrl.GetReloadLevel()){
        WorldManager::LoadLevel(WorldManager::level);
        
//         Network::SetRepo("https://raw.githubusercontent.com/Hammerill/Sand-Box2D-levels/main/levels");
// #ifdef Vita
//         Network::DownloadFile("ux0:Data/Sand-Box2D/levels", "index.json");
//         Network::DownloadFile("ux0:Data/Sand-Box2D/levels", "default_level/default_level.json");
// #else
//         Network::DownloadFile("./levels", "index.json");
//         Network::DownloadFile("./levels", "default_level/default_level.json");
// #endif
    }
    ///////////////////////////////////////////////////////

    for (int i = WorldManager::order.size() - 1; i >= 0; i--)
    { // Load order.
        WorldManager::objects.push_back(WorldManager::order[i]);
        WorldManager::objects[WorldManager::objects.size() - 1]->Register(WorldManager::world, WorldManager::renderer.GetRenderer());
        WorldManager::order.pop_back();        
    }

    for (size_t i = 0; i < WorldManager::objects.size(); i++)
    { // Out of bounds check.
        if (WorldManager::objects[i]->GetBody()->GetPosition().x > 100 ||
            WorldManager::objects[i]->GetBody()->GetPosition().y > 100 ||
            WorldManager::objects[i]->GetBody()->GetPosition().x < -100 ||
            WorldManager::objects[i]->GetBody()->GetPosition().y < -100 )
        {
            WorldManager::DeleteObject(i);
        }
    }

    return true;
}

int renderedItemsCount;
void WorldManager::Render()
{
    SDL_SetRenderDrawBlendMode(WorldManager::renderer.GetRenderer(), SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(WorldManager::renderer.GetRenderer(), 0x32, 0x32, 0x32, 0);
    SDL_RenderClear(WorldManager::renderer.GetRenderer());

    renderedItemsCount = 0;
    for (size_t i = 0; i < WorldManager::objects.size(); i++)   
    {
        if (WorldManager::objects[i]->Render(   WorldManager::renderer.GetRenderer(), 
                                                WorldManager::x_offset, 
                                                WorldManager::y_offset, 
                                                WorldManager::zoom,
                                                WorldManager::renderer.GetWidth(),
                                                WorldManager::renderer.GetHeight()))
        {
            renderedItemsCount++;
        }        
    }

    if (WorldManager::isDebug && Font::GetLoaded())
    {
        SDL_Point mouse = ctrl.GetMouse();

        std::vector<std::string> debugStrings;

        debugStrings.push_back("DEBUG");
        debugStrings.push_back("");
        debugStrings.push_back("Camera offset X = " + std::to_string(WorldManager::x_offset));
        debugStrings.push_back("Camera offset Y = " + std::to_string(WorldManager::y_offset));
        debugStrings.push_back("Zoom = " + std::to_string(WorldManager::zoom));
        debugStrings.push_back("Mouse X = " + std::to_string(mouse.x));
        debugStrings.push_back("Mouse Y = " + std::to_string(mouse.y));
        debugStrings.push_back("Delta X = " + std::to_string(ctrl.GetDeltaX()));
        debugStrings.push_back("Delta Y = " + std::to_string(ctrl.GetDeltaY()));
        debugStrings.push_back("IsMoving? = " + std::to_string(ctrl.GetIsMoving()));
        debugStrings.push_back("Delta pinch = " + std::to_string(ctrl.GetDeltaPinch()));
        debugStrings.push_back("IsPinching? = " + std::to_string(ctrl.GetIsPinching()));
        debugStrings.push_back("Zoom In = " + std::to_string(ctrl.GetZoomIn()));
        debugStrings.push_back("Zoom Out = " + std::to_string(ctrl.GetZoomOut()));
        debugStrings.push_back("Objects count = " + std::to_string(WorldManager::world->GetBodyCount()));
        debugStrings.push_back("Objects rendered = " + std::to_string(renderedItemsCount));

        WorldManager::RenderDebugScreen(debugStrings);        
    }

    SDL_RenderPresent(WorldManager::renderer.GetRenderer());
}

void WorldManager::Cycle()
{
    bool isRunning = true;

    if (WorldManager::speedCorrection)
    {
        while (isRunning)
        {
            WorldManager::a = SDL_GetTicks();
            WorldManager::delta = WorldManager::a - WorldManager::b;

            if (delta > 1000/60.0)
            {
                isRunning = WorldManager::Step();
                WorldManager::Render();
            }
        }
    }
    else
    {
        while (isRunning)
        {
            isRunning = WorldManager::Step();
            WorldManager::Render();
        }
    }
}

void WorldManager::LoadLevel(Level level)
{
    WorldManager::level = level;

    // CAMERA
    auto camera = WorldManager::level.GetCamera();

    if (camera.type == "static")
    {
        WorldManager::zoom = WorldManager::renderer.GetHeight() / camera.height;

        WorldManager::x_offset =    -(camera.x * WorldManager::zoom)
                                    +(WorldManager::renderer.GetWidth() / 2);

        WorldManager::y_offset =    -(camera.y * WorldManager::zoom)
                                    +(WorldManager::renderer.GetHeight() / 2);
    }
    /////////
    
    // OBJECTS
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    { // Remove current loaded objects
        WorldManager::DeleteObject(i);
    }
    
    auto objects = WorldManager::level.GetPObjects();
    for (size_t i = 0; i < objects.size(); i++)
    {
        WorldManager::AddObject(objects[i]);
    }
    //////////

    // CYCLES (everything other at the end of the Step())
    WorldManager::cyclesDelays = std::vector<int>();

    for (size_t i = 0; i < WorldManager::level.GetCycles().size(); i++)
    {
        WorldManager::cyclesDelays.push_back(1);
    }    
    /////////
}

SDL_Renderer* WorldManager::GetRenderer() { return WorldManager::renderer.GetRenderer(); }

void WorldManager::RenderDebugScreen(std::vector<std::string> debugStrings)
{
    float debugScale = 2;

    std::vector<int> debugWidths;
    for (size_t i = 0; i < debugStrings.size(); i++)
        debugWidths.push_back((debugStrings[i].size() + 2) * Font::FontWidth * debugScale);

    int debug_w = *std::max_element(debugWidths.begin(), debugWidths.end());
    int debug_h = (debugStrings.size() + 2) * Font::FontWidth * debugScale;

    SDL_Rect debugBg {0, 0, debug_w, debug_h};

    SDL_SetRenderDrawBlendMode(WorldManager::renderer.GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(WorldManager::renderer.GetRenderer(), 4, 4, 4, 0xA0);

    SDL_RenderFillRect(WorldManager::renderer.GetRenderer(), &debugBg);

    for (size_t i = 0; i < debugStrings.size(); i++)
    {
        Font::Render(WorldManager::renderer.GetRenderer(), debugStrings[i].c_str(), Font::FontWidth * debugScale, Font::FontWidth * debugScale * (i+1), debugScale);
    }
}

void WorldManager::CorrectOffset(SDL_Point mouse, float zoom_change)
{
    WorldManager::x_offset += ((WorldManager::x_offset - mouse.x) / WorldManager::zoom) * zoom_change;
    WorldManager::y_offset += ((WorldManager::y_offset - mouse.y) / WorldManager::zoom) * zoom_change;
}

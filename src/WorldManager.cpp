#include "WorldManager.h"

WorldManager::WorldManager(float move_speed, float zoom_speed)
{
    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;

    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    WorldManager::world = new b2World(gravity);

    WorldManager::objects = std::vector<BasePObj*>();

    Font::LoadFont("./assets/fonts/PressStart2P-vaV7.ttf");
}
WorldManager::~WorldManager()
{
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    {
        WorldManager::DeleteObject(i);
    }
    
    delete WorldManager::world;
}

void WorldManager::AddObject(BasePObj* obj) { WorldManager::order.push_back(obj); }
void WorldManager::DeleteObject(int index)
{
    delete WorldManager::objects[index]; // Call a destructor.
    WorldManager::objects.erase(WorldManager::objects.begin() + index); // Remove from vector.
}

void WorldManager::Step(Renderer* renderer, Controls ctrl, Controls old_ctrl)
{
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

    SDL_Point scr_center = {renderer->GetWidth() / 2, renderer->GetHeight() / 2};

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
    // Or no?
    if (ctrl.GetReloadLevel() && !old_ctrl.GetReloadLevel()){
        WorldManager::LoadLevel(WorldManager::level, renderer);
        
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
        WorldManager::objects[WorldManager::objects.size() - 1]->Register(WorldManager::world, renderer->GetRenderer());
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
}

int renderedItemsCount;
void WorldManager::Render(Renderer* renderer, Controls ctrl)
{
    renderedItemsCount = 0;
    for (size_t i = 0; i < WorldManager::objects.size(); i++)   
    {
        if (WorldManager::objects[i]->Render(   renderer->GetRenderer(), 
                                                WorldManager::x_offset, 
                                                WorldManager::y_offset, 
                                                WorldManager::zoom,
                                                renderer->GetWidth(),
                                                renderer->GetHeight()))
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

        WorldManager::RenderDebugScreen(debugStrings, renderer);        
    }
}

void WorldManager::LoadLevel(Level level, Renderer* renderer)
{
    WorldManager::level = level;

    // CAMERA
    auto camera = WorldManager::level.GetCamera();

    if (camera.type == "static")
    {
        WorldManager::zoom = renderer->GetHeight() / camera.height;

        WorldManager::x_offset =    -(camera.x * WorldManager::zoom)
                                    +(renderer->GetWidth() / 2);

        WorldManager::y_offset =    -(camera.y * WorldManager::zoom)
                                    +(renderer->GetHeight() / 2);
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

void WorldManager::RenderDebugScreen(std::vector<std::string> debugStrings, Renderer* renderer)
{
    float debugScale = 2;

    std::vector<int> debugWidths;
    for (size_t i = 0; i < debugStrings.size(); i++)
        debugWidths.push_back((debugStrings[i].size() + 2) * Font::FontWidth * debugScale);

    int debug_w = *std::max_element(debugWidths.begin(), debugWidths.end());
    int debug_h = (debugStrings.size() + 2) * Font::FontWidth * debugScale;

    SDL_Rect debugBg {0, 0, debug_w, debug_h};

    SDL_SetRenderDrawBlendMode(renderer->GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer->GetRenderer(), 4, 4, 4, 0xA0);

    SDL_RenderFillRect(renderer->GetRenderer(), &debugBg);

    for (size_t i = 0; i < debugStrings.size(); i++)
    {
        Font::Render(renderer->GetRenderer(), debugStrings[i].c_str(), Font::FontWidth * debugScale, Font::FontWidth * debugScale * (i+1), debugScale);
    }
}

void WorldManager::CorrectOffset(SDL_Point mouse, float zoom_change)
{
    WorldManager::x_offset += ((WorldManager::x_offset - mouse.x) / WorldManager::zoom) * zoom_change;
    WorldManager::y_offset += ((WorldManager::y_offset - mouse.y) / WorldManager::zoom) * zoom_change;
}

#include "WorldManager.h"

// #include "Settings.h"
// Settings settings = Settings("./settings.json", "./assets/default_settings.json");

// GONNA BE DEPRECATED SOON
Controls ctrl, old_ctrl;
///////////////////////////

WorldManager::WorldManager(int WINDOW_WIDTH, int WINDOW_HEIGHT, const char* path_to_font, bool fpsCorrection, const char* path_to_icon, int fullscreenScale, float move_speed, float zoom_speed)
{
    WorldManager::START_WINDOW_WIDTH = WINDOW_WIDTH;
    WorldManager::START_WINDOW_HEIGHT = WINDOW_HEIGHT;

    WorldManager::speedCorrection = fpsCorrection;

    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;

    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    WorldManager::world = new b2World(gravity);

    WorldManager::objects = std::vector<BasePObj*>();

    WorldManager::fullscreenScale = fullscreenScale;

    if (path_to_font != nullptr)
        Font::LoadFont(path_to_font);
    
    WorldManager::initVideo(path_to_icon);
}
WorldManager::~WorldManager()
{
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    {
        WorldManager::DeleteObject(i);
    }

    SDL_DestroyRenderer(WorldManager::renderer);
	SDL_DestroyWindow(WorldManager::window);
    delete[] WorldManager::world;
}

void WorldManager::initVideo(const char* path_to_icon)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    WorldManager::SCREEN_WIDTH = dm.w;
    WorldManager::SCREEN_HEIGHT = dm.h;

    if (WorldManager::START_WINDOW_WIDTH == 0 || WorldManager::START_WINDOW_HEIGHT == 0)
    {
        WorldManager::START_WINDOW_WIDTH = WorldManager::SCREEN_WIDTH / 1.5;
        WorldManager::START_WINDOW_HEIGHT = WorldManager::SCREEN_HEIGHT / 1.5;

        WorldManager::WINDOW_WIDTH = START_WINDOW_WIDTH;
        WorldManager::WINDOW_HEIGHT = START_WINDOW_HEIGHT;
    }
    else
    {
        WorldManager::WINDOW_WIDTH = START_WINDOW_WIDTH;
        WorldManager::WINDOW_HEIGHT = START_WINDOW_HEIGHT;
    }

    WorldManager::window = SDL_CreateWindow("Sand-Box2D", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WorldManager::WINDOW_WIDTH, WorldManager::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (path_to_icon != nullptr)
    {
        SDL_Surface* icon = IMG_Load(path_to_icon);
        SDL_SetWindowIcon(WorldManager::window, icon);
    }

    WorldManager::renderer = SDL_CreateRenderer(WorldManager::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(WorldManager::renderer, WorldManager::WINDOW_WIDTH, WorldManager::WINDOW_HEIGHT);
}

void WorldManager::AddObject(BasePObj* obj) { WorldManager::order.push_back(obj); }
void WorldManager::DeleteObject(int index)
{
    delete WorldManager::objects[index]; // Call a destructor.
    WorldManager::objects.erase(WorldManager::objects.begin() + index); // Remove from vector.
}

void ProceedPObj(JsonPObj* pObject, WorldManager* wm)
{
    // Gonna delete this hardcoded stuff later.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(wm->GetRenderer(), IMG_Load("assets/img/box.png"));
    // Also, I think that I should rewrite the whole JSON level loading system, bc rn it's very repetitive

    if (pObject->type == "platform")
    {
        JsonPObjPlatform* jsonPlatform = (JsonPObjPlatform*)pObject;
        PObjPlatform* platform = new PObjPlatform(  jsonPlatform->x1, jsonPlatform->y1,
                                                    jsonPlatform->x2, jsonPlatform->y2,
                                                    jsonPlatform->r, jsonPlatform->g, jsonPlatform->b);
        wm->AddObject(platform);
    }
    else if (pObject->type == "box")
    {
        JsonPObjBox* jsonBox = (JsonPObjBox*)pObject;
        PObjBox* box = new PObjBox( texture, jsonBox->x, jsonBox->y, 
                                    jsonBox->w, jsonBox->h, jsonBox->angle,
                                    jsonBox->vel_x, jsonBox->vel_y);
        wm->AddObject(box);
    }
    else if (pObject->type == "circle")
    {
        JsonPObjCircle* jsonCircle = (JsonPObjCircle*)pObject;
        PObjCircle* circle = new PObjCircle(jsonCircle->x, jsonCircle->y, jsonCircle->radius,
                                            jsonCircle->vel_x, jsonCircle->vel_y, 
                                            jsonCircle->r, jsonCircle->g, jsonCircle->b,
                                            jsonCircle->r_angle, jsonCircle->g_angle, jsonCircle->b_angle);
        wm->AddObject(circle);
    }
}

bool oldReload = false;
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
        WorldManager::isFullscreen = !WorldManager::isFullscreen;
        WorldManager::goFullscreen(WorldManager::isFullscreen);
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
        correctOffset(  ctrl.GetMouse(),
                        ctrl.GetDeltaPinch());
        WorldManager::zoom += ctrl.GetDeltaPinch();
    }

    SDL_Point scr_center = {WorldManager::WINDOW_WIDTH / 2, WorldManager::WINDOW_HEIGHT / 2};

    if (WorldManager::zoom <= 1)
    {
        correctOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        WorldManager::zoom - 1);
        WorldManager::zoom = 1;
    }
    else
    {
        correctOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        ctrl.GetZoomOut() * WorldManager::zoom_speed * -1 * WorldManager::zoom);
        WorldManager::zoom -= ctrl.GetZoomOut() * WorldManager::zoom_speed * WorldManager::zoom;
    }

    if (WorldManager::zoom >= 1000)
    {
        correctOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
                        1000 - WorldManager::zoom);
        WorldManager::zoom = 1000;
    }
    else
    {
        correctOffset(  ctrl.GetIsWheel() ? ctrl.GetMouse() : scr_center,
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
    for (size_t i = 0; i < WorldManager::cyclesDelays.size(); i++)
    {
        if (--(WorldManager::cyclesDelays[i]) <= 0)
        {
            for (size_t j = 0; j < WorldManager::level.GetCycles()[i].objects.size(); j++)
            {
                ProceedPObj(WorldManager::level.GetCycles()[i].objects[j], this);
            }
            WorldManager::cyclesDelays[i] = WorldManager::level.GetCycles()[i].delay;
        }
    }    
    /////////

    
    // LATER IT WILL BE CONSIDERED DEPRECATED AND DESTROYED
    if (ctrl.GetReloadLevel() && !oldReload){
        WorldManager::LoadLevel(WorldManager::level);
        
//         NetworkManager::SetRepo("https://raw.githubusercontent.com/Hammerill/Sand-Box2D-levels/main/levels");
// #ifdef Vita
//         NetworkManager::DownloadFile("ux0:Data/Sand-Box2D/levels", "index.json");
//         NetworkManager::DownloadFile("ux0:Data/Sand-Box2D/levels", "default_level/default_level.json");
// #else
//         NetworkManager::DownloadFile("./levels", "index.json");
//         NetworkManager::DownloadFile("./levels", "default_level/default_level.json");
// #endif
    }
    
    oldReload = ctrl.GetReloadLevel();
    ///////////////////////////////////////////////////////

    for (int i = WorldManager::order.size() - 1; i >= 0; i--)
    { // Load order.
        WorldManager::objects.push_back(WorldManager::order[i]);
        WorldManager::objects[WorldManager::objects.size() - 1]->Register(WorldManager::world, WorldManager::renderer);
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
    SDL_SetRenderDrawBlendMode(WorldManager::renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(WorldManager::renderer, 0x32, 0x32, 0x32, 0);
    SDL_RenderClear(WorldManager::renderer);

    renderedItemsCount = 0;
    for (size_t i = 0; i < WorldManager::objects.size(); i++)   
    {
        if (WorldManager::objects[i]->Render(   WorldManager::renderer, 
                                                WorldManager::x_offset, 
                                                WorldManager::y_offset, 
                                                WorldManager::zoom,
                                                WorldManager::SCREEN_WIDTH,
                                                WorldManager::SCREEN_HEIGHT))
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

    SDL_RenderPresent(WorldManager::renderer);
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
    if (WorldManager::level.camera.type == "static")
    {
        WorldManager::zoom = WorldManager::WINDOW_HEIGHT / WorldManager::level.camera.height;

        WorldManager::x_offset =    -(WorldManager::level.camera.x * WorldManager::zoom)
                                    +(WorldManager::WINDOW_WIDTH / 2);

        WorldManager::y_offset =    -(WorldManager::level.camera.y * WorldManager::zoom)
                                    +(WorldManager::WINDOW_HEIGHT / 2);
    }
    /////////
    
    // OBJECTS
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    { // Remove current loaded objects
        WorldManager::DeleteObject(i);
    }
    
    for (size_t i = 0; i < WorldManager::level.objects.size(); i++)
    {
        ProceedPObj(WorldManager::level.objects[i], this);
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

SDL_Renderer* WorldManager::GetRenderer() { return WorldManager::renderer; }

void WorldManager::goFullscreen(bool isToFullscreen)
{
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    WorldManager::SCREEN_WIDTH = dm.w;
    WorldManager::SCREEN_HEIGHT = dm.h;
    
    if (isToFullscreen)
    {
        SDL_SetWindowSize(WorldManager::window, WorldManager::SCREEN_WIDTH, WorldManager::SCREEN_HEIGHT);

        WorldManager::WINDOW_WIDTH = WorldManager::SCREEN_WIDTH / WorldManager::fullscreenScale;
        WorldManager::WINDOW_HEIGHT = WorldManager::SCREEN_HEIGHT / WorldManager::fullscreenScale;

        SDL_RenderSetLogicalSize(WorldManager::renderer, WorldManager::WINDOW_WIDTH, WorldManager::WINDOW_HEIGHT);

        SDL_SetWindowFullscreen(WorldManager::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        WorldManager::WINDOW_WIDTH = START_WINDOW_WIDTH;
        WorldManager::WINDOW_HEIGHT = START_WINDOW_HEIGHT;

        SDL_SetWindowSize(WorldManager::window, WorldManager::WINDOW_WIDTH, WorldManager::WINDOW_HEIGHT);
        SDL_RenderSetLogicalSize(WorldManager::renderer, WorldManager::WINDOW_WIDTH, WorldManager::WINDOW_HEIGHT);

        SDL_SetWindowFullscreen(WorldManager::window, 0);

        // SDL_ShowSimpleMessageBox(0, 0, settings.Get("test").asString().c_str(), 0);
    }
}

void WorldManager::RenderDebugScreen(std::vector<std::string> debugStrings)
{
    float debugScale = 2;

    std::vector<int> debugWidths;
    for (size_t i = 0; i < debugStrings.size(); i++)
        debugWidths.push_back((debugStrings[i].size() + 2) * Font::FontWidth * debugScale);

    int debug_w = *std::max_element(debugWidths.begin(), debugWidths.end());
    int debug_h = (debugStrings.size() + 2) * Font::FontWidth * debugScale;

    SDL_Rect debugBg {0, 0, debug_w, debug_h};

    SDL_SetRenderDrawBlendMode(WorldManager::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(WorldManager::renderer, 4, 4, 4, 0xA0);

    SDL_RenderFillRect(WorldManager::renderer, &debugBg);

    for (size_t i = 0; i < debugStrings.size(); i++)
    {
        Font::Render(WorldManager::renderer, debugStrings[i].c_str(), Font::FontWidth * debugScale, Font::FontWidth * debugScale * (i+1), debugScale);
    }
}

void WorldManager::correctOffset(SDL_Point mouse, float zoom_change)
{
    WorldManager::x_offset += ((WorldManager::x_offset - mouse.x) / WorldManager::zoom) * zoom_change;
    WorldManager::y_offset += ((WorldManager::y_offset - mouse.y) / WorldManager::zoom) * zoom_change;
}

#include "WorldManager.h"

WorldManager::WorldManager(std::string path_to_def_texture, int physics_quality, int moving_inertia_frames, float move_speed, float zoom_speed)
{
    WorldManager::path_to_def_texture = path_to_def_texture;
    WorldManager::physics_quality = physics_quality;
    WorldManager::moving_inertia_frames = moving_inertia_frames;
    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;

    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    WorldManager::world = new b2World(gravity);

    WorldManager::objects = std::vector<BasePObj*>();

    AnimationManager::InitAnim(ANIM_WORLD_MANAGER_INIT);
}
WorldManager::~WorldManager()
{
    /// @brief Delete all the objects.
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    {
        WorldManager::DeleteObject(i);
    }
    /// @brief Unload all the textures.
    for (std::map<std::string, SDL_Texture*>::iterator itr = WorldManager::textures.begin(); itr != WorldManager::textures.end(); itr++)
    {
        SDL_DestroyTexture(itr->second);
    }
    /// @brief Destroy the world.
    delete WorldManager::world;
}

void WorldManager::LoadLevel(Level level, Renderer* rr)
{
    WorldManager::level = level;

    WorldManager::textures[""] = SDL_CreateTextureFromSurface(rr->GetRenderer(), IMG_Load(WorldManager::path_to_def_texture.c_str()));
    
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

    // CAMERA
    auto camera = WorldManager::level.GetCamera();

    WorldManager::zoom = rr->GetWindowParams().height / camera.height;

    switch (camera.type)
    {
    case CAMERA_TYPE_STATIC:
        WorldManager::x_offset =    -(camera.x * WorldManager::zoom)
                                    +(rr->GetWindowParams().width / 2);

        WorldManager::y_offset =    -(camera.y * WorldManager::zoom)
                                    +(rr->GetWindowParams().height / 2);
        break;
    
    case CAMERA_TYPE_ATTACHED:
        {
            b2Vec2 pos;
            BasePObj* pobj = WorldManager::level.GetObjectById(camera.attached_id, WorldManager::order);

            if (pobj == nullptr)
                pos = {0, 0};
            else
            {
                pos = {
                    pobj->GetParam("x").asFloat(),
                    pobj->GetParam("y").asFloat()
                };
            }

            WorldManager::x_offset =    -(pos.x * WorldManager::zoom)
                                        +(rr->GetWindowParams().width / 2);

            WorldManager::y_offset =    -(pos.y * WorldManager::zoom)
                                        +(rr->GetWindowParams().height / 2);
        }
        break;
    
    default:
        break;
    }
    /////////

    // CYCLES (everything other at the end of the Step())
    WorldManager::cyclesDelays = std::vector<int>();

    for (size_t i = 0; i < WorldManager::level.GetCycles().size(); i++)
    {
        WorldManager::cyclesDelays.push_back(1);
    }    
    /////////

    // ACTIONS (everything other at control handling sector)
    WorldManager::actions = WorldManager::level.GetActions();
    //////////
}

void WorldManager::AddObject(BasePObj* obj)
{
    WorldManager::order.push_back(obj);
}
void WorldManager::DeleteObject(int index)
{
    delete WorldManager::objects[index]; // Call a destructor.
    WorldManager::objects.erase(WorldManager::objects.begin() + index); // Remove from vector.
}

template<typename T>
double GetAverage(std::vector<T> const& v)
{
    if (v.empty())
        return 0;
 
    return std::reduce(v.begin(), v.end(), 0.0) / v.size();
}

// Used for decreasing actions code
void HandleActionCtrl(bool old, bool now, Json::Value key, Level level, std::vector<BasePObj*> objects)
{
    if (now && !old)
        level.PerformAction(key["keydown_once"], objects);
    else if (now)
        level.PerformAction(key["keydown_hold"], objects);
    else if (!now && old)
        level.PerformAction(key["keyup"], objects);
}

std::vector<int> last_frames_speed_x = std::vector<int>();
std::vector<int> last_frames_speed_y = std::vector<int>();

WindowParams old_wparams, now_wparams;
float zoomChange, zoomChangeCoeff;

void WorldManager::Step(Renderer* rr, Controls ctrl, Controls old_ctrl)
{
    old_wparams = now_wparams;
    now_wparams = rr->GetWindowParams();

    if (old_wparams.width != 0 && old_wparams.height != 0)
    {
        if (old_wparams.width != now_wparams.width)
            WorldManager::x_offset += (now_wparams.width - old_wparams.width) / 2;
        
        if (old_wparams.height != now_wparams.height)
        {
            WorldManager::y_offset += (now_wparams.height - old_wparams.height) / 2;

            zoomChangeCoeff = (float)now_wparams.height / (float)old_wparams.height;
            zoomChange = (WorldManager::zoom * zoomChangeCoeff) - WorldManager::zoom;

            CorrectOffset({now_wparams.width / 2, now_wparams.height / 2}, zoomChange);
            WorldManager::zoom += zoomChange;
        }
    }

    if (ctrl.Debug() && !old_ctrl.Debug())
        WorldManager::isDebug = !WorldManager::isDebug;

    if (WorldManager::level.GetCamera().move)
    {
        WorldManager::y_offset += ctrl.MoveUp() * WorldManager::move_speed;
        WorldManager::x_offset -= ctrl.MoveRight() * WorldManager::move_speed;
        WorldManager::y_offset -= ctrl.MoveDown() * WorldManager::move_speed;
        WorldManager::x_offset += ctrl.MoveLeft() * WorldManager::move_speed;

        if (last_frames_speed_x.size() > (size_t)(WorldManager::moving_inertia_frames))
        {
            last_frames_speed_x.erase(last_frames_speed_x.begin());
            last_frames_speed_y.erase(last_frames_speed_y.begin());
        }

        if (ctrl.IsMoving())
        {
            WorldManager::x_offset += (ctrl.GetMouse().x - old_ctrl.GetMouse().x);
            WorldManager::y_offset += (ctrl.GetMouse().y - old_ctrl.GetMouse().y);
            last_frames_speed_x.push_back(ctrl.GetMouse().x - old_ctrl.GetMouse().x);
            last_frames_speed_y.push_back(ctrl.GetMouse().y - old_ctrl.GetMouse().y);
        }
        else
        {
            WorldManager::x_offset += GetAverage(last_frames_speed_x);
            WorldManager::y_offset += GetAverage(last_frames_speed_y);
            last_frames_speed_x.push_back(0);
            last_frames_speed_y.push_back(0);
        }
    }

    SDL_Point scr_center = {rr->GetWindowParams().width / 2, rr->GetWindowParams().height / 2};

    if (WorldManager::level.GetCamera().zoom)
    {
        if (ctrl.IsPinching() && ((WorldManager::zoom + (ctrl.GetPinch() - old_ctrl.GetPinch())) > 10))
        {
            CorrectOffset(  WorldManager::level.GetCamera().move
                                ? ctrl.GetMouse() : scr_center,
                            (ctrl.GetPinch() - old_ctrl.GetPinch()));
            WorldManager::zoom += (ctrl.GetPinch() - old_ctrl.GetPinch());
        }

        if (WorldManager::zoom <= 1)
        {
            CorrectOffset(  ctrl.IsWheel() && WorldManager::level.GetCamera().move
                                ? ctrl.GetMouse() : scr_center,
                            WorldManager::zoom - 1);
            WorldManager::zoom = 1;
        }
        else
        {
            CorrectOffset(  ctrl.IsWheel() && WorldManager::level.GetCamera().move
                                ? ctrl.GetMouse() : scr_center,
                            ctrl.ZoomOut() * WorldManager::zoom_speed * -1 * WorldManager::zoom);
            WorldManager::zoom -= ctrl.ZoomOut() * WorldManager::zoom_speed * WorldManager::zoom;
        }

        if (WorldManager::zoom >= 1000)
        {
            CorrectOffset(  ctrl.IsWheel() && WorldManager::level.GetCamera().move
                                ? ctrl.GetMouse() : scr_center,
                            1000 - WorldManager::zoom);
            WorldManager::zoom = 1000;
        }
        else
        {
            CorrectOffset(  ctrl.IsWheel() && WorldManager::level.GetCamera().move
                                ? ctrl.GetMouse() : scr_center,
                            ctrl.ZoomIn() * WorldManager::zoom_speed * WorldManager::zoom);
            WorldManager::zoom += ctrl.ZoomIn() * WorldManager::zoom_speed * WorldManager::zoom;
        }
    }

    if (WorldManager::level.GetCamera().type == CAMERA_TYPE_ATTACHED)
    {
        b2Vec2 pos;
        BasePObj* pobj = WorldManager::level.GetObjectById(WorldManager::level.GetCamera().attached_id, WorldManager::objects);

        if (pobj == nullptr)
            pos = {x_offset, y_offset};
        else
        {
            pos = {
                pobj->GetParam("x").asFloat() * WorldManager::zoom + x_offset,
                pobj->GetParam("y").asFloat() * WorldManager::zoom + y_offset
            };
        }
        
        WorldManager::x_offset += (scr_center.x - pos.x) * ((100 - WorldManager::level.GetCamera().attached_remain) / 100.0);
        WorldManager::y_offset += (scr_center.y - pos.y) * ((100 - WorldManager::level.GetCamera().attached_remain) / 100.0);
    }

    // ACTIONS
    HandleActionCtrl(old_ctrl.ActionUp(), ctrl.ActionUp(), WorldManager::actions["up"],WorldManager::level, WorldManager::objects);
    HandleActionCtrl(old_ctrl.ActionRight(), ctrl.ActionRight(), WorldManager::actions["right"], WorldManager::level, WorldManager::objects);
    HandleActionCtrl(old_ctrl.ActionDown(), ctrl.ActionDown(), WorldManager::actions["down"], WorldManager::level, WorldManager::objects);
    HandleActionCtrl(old_ctrl.ActionLeft(), ctrl.ActionLeft(), WorldManager::actions["left"], WorldManager::level, WorldManager::objects);
    
    HandleActionCtrl(old_ctrl.ActionEnter(), ctrl.ActionEnter(), WorldManager::actions["enter"], WorldManager::level, WorldManager::objects);
    //////////

    WorldManager::world->Step(1.0f / 60.0f, WorldManager::physics_quality * 3, WorldManager::physics_quality);

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
    // No, it will be moved to the PauseMenu
    if (ctrl.ReloadLevel() && !old_ctrl.ReloadLevel()){
        WorldManager::LoadLevel(WorldManager::level, rr);
        
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

    // POBJECTS REGISTRATION
    for (int i = WorldManager::order.size() - 1; i >= 0; i--)
    {
        WorldManager::objects.push_back(WorldManager::order[i]);

        auto pobj = WorldManager::objects[WorldManager::objects.size() - 1];

        pobj->Register(WorldManager::world, rr->GetRenderer(), WorldManager::textures);
        
        WorldManager::order.pop_back();        
    }
    ////////////////////////

    // OUT OF BORDERS CHECK
    for (size_t i = 0; i < WorldManager::objects.size(); i++)
    {
        if (
               (WorldManager::objects[i]->GetBody()->GetPosition().x > 100 ||
                WorldManager::objects[i]->GetBody()->GetPosition().y > 100 ||
                WorldManager::objects[i]->GetBody()->GetPosition().x < -100 ||
                WorldManager::objects[i]->GetBody()->GetPosition().y < -100)
            &&
                !WorldManager::objects[i]->GetParam("undeletable").asBool()
            )
        {
            WorldManager::DeleteObject(i);
        }
    }
    ///////////////////////

    // ANIMATIONS
    AnimationManager::StepAnim(ANIM_WORLD_MANAGER_INIT);
    /////////////
}

int renderedItemsCount;
void WorldManager::Render(Renderer* rr, Controls ctrl)
{
    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(rr->GetRenderer(), 0x32, 0x32, 0x32, 0); //BG OPTION TO WORK ON
    SDL_RenderClear(rr->GetRenderer());

    renderedItemsCount = 0;
    for (size_t i = 0; i < WorldManager::objects.size(); i++)   
    {
        if (WorldManager::objects[i]->Render(   rr->GetRenderer(), 
                                                WorldManager::x_offset, 
                                                WorldManager::y_offset, 
                                                WorldManager::zoom,
                                                rr->GetWindowParams().width,
                                                rr->GetWindowParams().height))
        {
            renderedItemsCount++;
        }        
    }

    if (WorldManager::isDebug && rr->GetFont()->GetLoaded())
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
        debugStrings.push_back("IsMoving? = " + std::to_string(ctrl.IsMoving()));
        debugStrings.push_back("Pinch = " + std::to_string(ctrl.GetPinch()));
        debugStrings.push_back("IsPinching? = " + std::to_string(ctrl.IsPinching()));
        debugStrings.push_back("Zoom In = " + std::to_string(ctrl.ZoomIn()));
        debugStrings.push_back("Zoom Out = " + std::to_string(ctrl.ZoomOut()));
        debugStrings.push_back("Zoom Change coeff = " + std::to_string(zoomChangeCoeff));
        debugStrings.push_back("Zoom Change = " + std::to_string(zoomChange));
        debugStrings.push_back("Objects count = " + std::to_string(WorldManager::world->GetBodyCount()));
        debugStrings.push_back("Objects rendered = " + std::to_string(renderedItemsCount));
        debugStrings.push_back("Loaded textures = " + std::to_string(WorldManager::textures.size()));
        debugStrings.push_back("");
        debugStrings.push_back("ACTIONS");
        debugStrings.push_back("Up = " + std::to_string(ctrl.ActionUp()));
        debugStrings.push_back("Right = " + std::to_string(ctrl.ActionRight()));
        debugStrings.push_back("Down = " + std::to_string(ctrl.ActionDown()));
        debugStrings.push_back("Left = " + std::to_string(ctrl.ActionLeft()));
        debugStrings.push_back("Enter = " + std::to_string(ctrl.ActionEnter()));

        WorldManager::RenderDebugScreen(debugStrings, rr);        
    }
    
    AnimationManager::RenderAnim(ANIM_WORLD_MANAGER_INIT, rr);
}

void WorldManager::RenderDebugScreen(std::vector<std::string> debugStrings, Renderer* rr)
{
    float debugScale = 2;
    int fontWidth = rr->GetFont()->FontWidth;

    std::vector<int> debugWidths;
    for (size_t i = 0; i < debugStrings.size(); i++)
        debugWidths.push_back((debugStrings[i].size() + 2) * fontWidth * debugScale);

    int debug_w = *std::max_element(debugWidths.begin(), debugWidths.end());
    int debug_h = (debugStrings.size() + 2) * fontWidth * debugScale;

    SDL_Rect debugBg {0, 0, debug_w, debug_h};

    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rr->GetRenderer(), 4, 4, 4, 0xA0);

    SDL_RenderFillRect(rr->GetRenderer(), &debugBg);

    for (size_t i = 0; i < debugStrings.size(); i++)
    {
        rr->RenderText(debugStrings[i].c_str(), fontWidth * debugScale, fontWidth * debugScale * (i+1), debugScale);
    }
}

void WorldManager::CorrectOffset(SDL_Point mouse, float zoom_change)
{
    WorldManager::x_offset += ((WorldManager::x_offset - mouse.x) / WorldManager::zoom) * zoom_change;
    WorldManager::y_offset += ((WorldManager::y_offset - mouse.y) / WorldManager::zoom) * zoom_change;
}

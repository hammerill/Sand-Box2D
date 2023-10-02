#include "WorldManager.h"

WorldManager::WorldManager(std::string path_to_def_texture, int physics_quality, int moving_inertia_frames, float move_speed, float zoom_speed)
{
    WorldManager::path_to_def_texture = path_to_def_texture;
    WorldManager::physics_quality = physics_quality;
    WorldManager::moving_inertia_frames = moving_inertia_frames;
    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;
}
WorldManager::~WorldManager()
{
    WorldManager::FreeMemory();
}

void WorldManager::FreeMemory()
{
    // Delete all the objects.
    for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
    {
        WorldManager::DeleteObject(i);
    }
    WorldManager::objects.clear();
    WorldManager::order.clear();

    // Unload all the textures.
    for (std::map<std::string, SDL_Texture*>::iterator itr = WorldManager::textures.begin(); itr != WorldManager::textures.end(); itr++)
    {
        SDL_DestroyTexture(itr->second);
    }
    WorldManager::textures.clear();

    // Destroy the world.
    if (WorldManager::world != nullptr)
    {
        delete WorldManager::world;
        WorldManager::world = nullptr;
    }
}

bool fadeout = false;

void WorldManager::LoadLevel(Level level, Renderer* rr)
{
    fadeout = false;
    WorldManager::level = level;

    WorldManager::FreeMemory();

    WorldManager::world = new b2World(WorldManager::level.GetOptions().gravity);
    WorldManager::objects = std::vector<BasePObj*>();

    WorldManager::textures[""] = SDL_CreateTextureFromSurface(rr->GetRenderer(), IMG_Load(WorldManager::path_to_def_texture.c_str()));

    // OBJECTS
    auto objects = WorldManager::level.GetPObjects();
    for (size_t i = 0; i < objects.size(); i++)
    {
        WorldManager::AddObject(objects[i]);
    }
    //////////

    // CAMERA
    auto camera = WorldManager::level.GetCamera();

    WorldManager::zoom = rr->GetHeight() / camera.height;

    switch (camera.type)
    {
    case CAMERA_TYPE_STATIC:
        WorldManager::x_offset =    -(camera.x * WorldManager::zoom)
                                    +(rr->GetWidth() / 2);

        WorldManager::y_offset =    -(camera.y * WorldManager::zoom)
                                    +(rr->GetHeight() / 2);
        break;

    case CAMERA_TYPE_ATTACHED:
        {
            b2Vec2 pos;
            BasePObj* pobj = WorldManager::level.GetObjectById(camera.attached_id, WorldManager::order);

            if (pobj == nullptr)
                pos = {0, 0};
            else
            {
                pos = {pobj->GetX(), pobj->GetY()};
            }

            WorldManager::x_offset =    -(pos.x * WorldManager::zoom)
                                        +(rr->GetWidth() / 2);

            WorldManager::y_offset =    -(pos.y * WorldManager::zoom)
                                        +(rr->GetHeight() / 2);
        }
        break;

    default:
        break;
    }
    /////////

    // CYCLES
    WorldManager::cyclesDelays = std::vector<int>();

    for (size_t i = 0; i < WorldManager::level.GetCyclesCount(); i++)
    {
        WorldManager::cyclesDelays.push_back(1);
    }
    /////////

    // ACTIONS
    WorldManager::actions = WorldManager::level.GetActions();
    //////////

    // ANIMATIONS
    AnimationManager::InitAnim(ANIM_WORLD_MANAGER_INIT);
    /////////////
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
void HandleActionCtrl(bool old, bool now, Json::Value& key, int index, Level& level, std::vector<BasePObj*>& objects)
{
    if (!now && !old)
        return;

    const char* key_button;
    switch (index)
    {
    case 0:
        key_button = "up";
        break;
    case 1:
        key_button = "right";
        break;
    case 2:
        key_button = "down";
        break;
    case 3:
        key_button = "left";
        break;

    default:
        key_button = "enter";
        break;
    }

    if (now && !old)
        level.PerformAction(key[key_button]["keydown_once"], objects);
    else if (now)
        level.PerformAction(key[key_button]["keydown_hold"], objects);
    else if (!now && old)
        level.PerformAction(key[key_button]["keyup"], objects);
}

std::vector<int> last_frames_speed_x = std::vector<int>();
std::vector<int> last_frames_speed_y = std::vector<int>();

WindowParams old_wparams, now_wparams;
float zoomChange, zoomChangeCoeff;

// std::vector<double> frame_times;

JsonOptions options;

// bool trigger = false;

bool WorldManager::Step(Renderer* rr, Controls ctrl, Controls old_ctrl)
{
    // auto previous_frame = std::chrono::high_resolution_clock::now();
    // auto duration_measure = std::chrono::duration<double, std::milli>();
    // frame_times = std::vector<double>();

    // 1. BOX2D (physics) STEP
    WorldManager::world->Step(1.0f / 60.0f, WorldManager::physics_quality * 3, WorldManager::physics_quality);
    //////////////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 2. GAME WINDOW UPDATE ON RESOLUTION CHANGE
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
    /////////////////////////////////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 3. FADEOUT HANDLING
    if (fadeout)
        return AnimationManager::StepAnim(ANIM_FADE_OUT);
    else if (ctrl.Pause())
    {
        fadeout = true;
        AnimationManager::InitAnim(ANIM_FADE_OUT);
    }
    //////////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 4. BUTTONS
    auto camera = WorldManager::level.GetCamera();

    if (ctrl.Debug() && !old_ctrl.Debug())
        WorldManager::isDebug = !WorldManager::isDebug;

    if (camera.move)
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
    /////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 5. ZOOM WHEEL/PINCH
    SDL_Point scr_center = {rr->GetWidth() / 2, rr->GetHeight() / 2};

    if (camera.zoom)
    {
        if (ctrl.IsPinching() && ((WorldManager::zoom * ((float)ctrl.GetPinch() / (float)old_ctrl.GetPinch())) >= 1))
        {
            CorrectOffset(  camera.move
                                ? ctrl.GetMouse() : scr_center,
                            (WorldManager::zoom * ((float)ctrl.GetPinch() / (float)old_ctrl.GetPinch())) - WorldManager::zoom);
            WorldManager::zoom *= ((float)ctrl.GetPinch() / (float)old_ctrl.GetPinch());
        }

        if (ctrl.ZoomIn() != ctrl.ZoomOut())
        {
            if (WorldManager::zoom <= 1)
            {
                CorrectOffset(  ctrl.IsWheel() && camera.move
                                    ? ctrl.GetMouse() : scr_center,
                                WorldManager::zoom - 1);
                WorldManager::zoom = 1;
            }
            else
            {
                CorrectOffset(  ctrl.IsWheel() && camera.move
                                    ? ctrl.GetMouse() : scr_center,
                                ctrl.ZoomOut() * WorldManager::zoom_speed * -1 * WorldManager::zoom);
                WorldManager::zoom -= ctrl.ZoomOut() * WorldManager::zoom_speed * WorldManager::zoom;
            }

            if (WorldManager::zoom >= 1000)
            {
                CorrectOffset(  ctrl.IsWheel() && camera.move
                                    ? ctrl.GetMouse() : scr_center,
                                1000 - WorldManager::zoom);
                WorldManager::zoom = 1000;
            }
            else
            {
                CorrectOffset(  ctrl.IsWheel() && camera.move
                                    ? ctrl.GetMouse() : scr_center,
                                ctrl.ZoomIn() * WorldManager::zoom_speed * WorldManager::zoom);
                WorldManager::zoom += ctrl.ZoomIn() * WorldManager::zoom_speed * WorldManager::zoom;
            }
        }
    }
    //////////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 6. CAMERA TYPE PROCESSING
    if (camera.type == CAMERA_TYPE_ATTACHED)
    {
        b2Vec2 pos;
        BasePObj* pobj = WorldManager::level.GetObjectById(camera.attached_id, WorldManager::objects);

        if (pobj == nullptr)
            pos = {x_offset, y_offset};
        else
        {
            pos = {
                pobj->GetX() * WorldManager::zoom + x_offset,
                pobj->GetY() * WorldManager::zoom + y_offset
            };
        }

        WorldManager::x_offset += (scr_center.x - pos.x) * ((100 - camera.attached_remain) / 100.0);
        WorldManager::y_offset += (scr_center.y - pos.y) * ((100 - camera.attached_remain) / 100.0);
    }
    ////////////////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 7. ACTIONS
    HandleActionCtrl(old_ctrl.ActionUp(), ctrl.ActionUp(), WorldManager::actions, 0, WorldManager::level, WorldManager::objects);
    HandleActionCtrl(old_ctrl.ActionRight(), ctrl.ActionRight(), WorldManager::actions, 1, WorldManager::level, WorldManager::objects);
    HandleActionCtrl(old_ctrl.ActionDown(), ctrl.ActionDown(), WorldManager::actions, 2, WorldManager::level, WorldManager::objects);
    HandleActionCtrl(old_ctrl.ActionLeft(), ctrl.ActionLeft(), WorldManager::actions, 3, WorldManager::level, WorldManager::objects);

    HandleActionCtrl(old_ctrl.ActionEnter(), ctrl.ActionEnter(), WorldManager::actions, 4, WorldManager::level, WorldManager::objects);
    /////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 8. CYCLES
    for (size_t i = 0; i < WorldManager::cyclesDelays.size(); i++)
    {
        if (--(WorldManager::cyclesDelays[i]) <= 0)
        {
            auto cycle = WorldManager::level.GetCycle(i);

            WorldManager::cyclesDelays[i] = cycle.delay;

            for (size_t j = 0; j < cycle.objects.size(); j++)
            {
                WorldManager::AddObject(cycle.objects[j]);
            }
        }
    }
    ////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 9. POBJECTS REGISTRATION
    for (int i = WorldManager::order.size() - 1; i >= 0; i--)
    {
        WorldManager::objects.push_back(WorldManager::order[i]);

        auto pobj = WorldManager::objects[WorldManager::objects.size() - 1];

        pobj->Register(WorldManager::world, rr, WorldManager::textures);

        WorldManager::order.pop_back();
    }
    ///////////////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 10. OUT OF BORDERS CHECK
    options = WorldManager::level.GetOptions();
    for (size_t i = 0; i < WorldManager::objects.size(); i++)
    {
        if (
                (options.border_width != 0 &&
                    (WorldManager::objects[i]->GetX() > options.border_width / 2 ||
                        WorldManager::objects[i]->GetX() < options.border_width / -2)
                )
                ||
                (options.border_height != 0 &&
                    (WorldManager::objects[i]->GetY() > options.border_height / 2 ||
                        WorldManager::objects[i]->GetY() < options.border_height / -2)
                )
            )
        {
            if (!WorldManager::objects[i]->GetParam("undeletable").asBool())
                WorldManager::DeleteObject(i);
        }
    }
    ///////////////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());
    // previous_frame = std::chrono::high_resolution_clock::now();

    // 11. ANIMATIONS
    AnimationManager::StepAnim(ANIM_WORLD_MANAGER_INIT);
    /////////////////
    // duration_measure = std::chrono::high_resolution_clock::now() - previous_frame;
    // frame_times.push_back(duration_measure.count());

    // NETWORK SAMPLE CODE (for me to not forget)
//         Network::SetRepo("https://raw.githubusercontent.com/Hammerill/Sand-Box2D-levels/main/levels");
// #ifdef Vita
//         Network::DownloadFile("ux0:Data/Sand-Box2D/levels", "index.json");
//         Network::DownloadFile("ux0:Data/Sand-Box2D/levels", "default_level/default_level.json");
// #else
//         Network::DownloadFile("./levels", "index.json");
//         Network::DownloadFile("./levels", "default_level/default_level.json");
// #endif
    ///////////////////////////////////////////////////////

    return true;
}

int renderedItemsCount;
void WorldManager::Render(Renderer* rr, Controls ctrl)
{
    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(
        rr->GetRenderer(),
        options.bg_r,
        options.bg_g,
        options.bg_b,
        0
    );
    SDL_RenderClear(rr->GetRenderer());

    renderedItemsCount = 0;
    for (size_t i = 0; i < WorldManager::objects.size(); i++)
    {
        if (WorldManager::objects[i]->Render(
                rr,
                WorldManager::x_offset,
                WorldManager::y_offset,
                WorldManager::zoom))
        {
            renderedItemsCount++;
        }
    }

    if (WorldManager::isDebug && rr->GetFont(Translations::GetJp())->GetLoaded())
    {
        std::vector<std::string> debugStrings;

        debugStrings.push_back("DEBUG");
        debugStrings.push_back("");
        debugStrings.push_back("Camera offset X = " + std::to_string(WorldManager::x_offset));
        debugStrings.push_back("Camera offset Y = " + std::to_string(WorldManager::y_offset));
        debugStrings.push_back("Zoom = " + std::to_string(WorldManager::zoom));
        debugStrings.push_back("Objects count = " + std::to_string(WorldManager::world->GetBodyCount()));
        debugStrings.push_back("Objects rendered = " + std::to_string(renderedItemsCount));
        debugStrings.push_back("Loaded textures = " + std::to_string(WorldManager::textures.size()));
        // debugStrings.push_back("");
        // debugStrings.push_back("Control test = " + std::to_string(ctrl.GetControlTest()));

        // debugStrings.push_back("STEP TIMES");
        // debugStrings.push_back("01. Physics = " + std::to_string(frame_times[0]));
        // debugStrings.push_back("02. Window change = " + std::to_string(frame_times[1]));
        // debugStrings.push_back("03. Fadeout handling = " + std::to_string(frame_times[2]));
        // debugStrings.push_back("04. Buttons = " + std::to_string(frame_times[3]));
        // debugStrings.push_back("05. Zoom = " + std::to_string(frame_times[4]));
        // debugStrings.push_back("06. Camera type = " + std::to_string(frame_times[5]));
        // debugStrings.push_back("07. Actions = " + std::to_string(frame_times[6]));
        // debugStrings.push_back("08. Cycles = " + std::to_string(frame_times[7]));
        // debugStrings.push_back("09. PObj registration = " + std::to_string(frame_times[8]));
        // debugStrings.push_back("10. OOB check = " + std::to_string(frame_times[9]));
        // debugStrings.push_back("11. Animations = " + std::to_string(frame_times[10]));

        WorldManager::RenderDebugScreen(debugStrings, rr);
    }

    AnimationManager::RenderAnim(ANIM_WORLD_MANAGER_INIT, rr);

    if (fadeout)
        AnimationManager::RenderAnim(ANIM_FADE_OUT, rr);
    
    // if (trigger)
    // {
    //     uint8_t* pixels = (uint8_t*)(malloc(rr->GetWidth() * rr->GetHeight() * 4));
    //     SDL_RenderReadPixels(
    //         rr->GetRenderer(),
    //         NULL,
    //         SDL_PIXELFORMAT_ABGR8888,
    //         pixels,
    //         rr->GetWidth() * 4
    //     );
    //     SDL_Surface* win_surface = SDL_CreateRGBSurfaceFrom(
    //         pixels,
    //         rr->GetWidth(),
    //         rr->GetHeight(),
    //         32,
    //         rr->GetWidth() * 4,
    //         0x000000FF,
    //         0x0000FF00,
    //         0x00FF0000,
    //         0xFF000000
    //     );

    //     int blur_extent = 2;

    //     for (int y = 0; y < win_surface->h; y++)
    //     {
    //         for (int x = 0; x < (win_surface->pitch / 4); x++)
    //         {
    //             Uint32 color = ((Uint32*)win_surface->pixels)[(y * (win_surface->pitch / 4)) + x];

    //             //SDL_GetRGBA() is a method for getting color
    //             //components from a 32 bit color
    //             Uint8 r = 0, g = 0, b = 0, a = 0;
    //             SDL_GetRGBA(color, win_surface->format, &r, &g, &b, &a);

    //             Uint32 rb = 0, gb = 0, bb = 0, ab = 0;

    //             //Within the two for-loops below, colors of adjacent pixels are added up

    //             for (int yo = -blur_extent; yo <= blur_extent; yo++)
    //             {
    //                 for (int xo = -blur_extent; xo <= blur_extent; xo++)
    //                 {
    //                     if (y + yo >= 0
    //                         && x + xo >= 0
    //                         && y + yo < win_surface->h
    //                         && x + xo < (win_surface->pitch / 4)
    //                     )
    //                     {
    //                         Uint32 colOth = ((Uint32*)win_surface->pixels)[((y + yo)
    //                                                 * (win_surface->pitch / 4)) + (x + xo)];

    //                         Uint8 ro = 0, go = 0, bo = 0, ao = 0;
    //                         SDL_GetRGBA(colOth, win_surface->format, &ro, &go, &bo, &ao);

    //                         rb += ro;
    //                         gb += go;
    //                         bb += bo;
    //                         ab += ao;
    //                     }
    //                 }
    //             }

    //             //The sum is then, divided by the total number of
    //             //pixels present in a block of blur radius

    //             //For blur_extent 1, it will be 9
    //             //For blur_extent 2, it will be 25
    //             //and so on...

    //             //In this way, we are getting the average of
    //             //all the pixels in a block of blur radius

    //             //(((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)) calculates
    //             //the total number of pixels present in a block of blur radius

    //             r = (Uint8)(rb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
    //             g = (Uint8)(gb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
    //             b = (Uint8)(bb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
    //             a = (Uint8)(ab / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));

    //             //Bit shifting color bits to form a 32 bit proper colour
    //             color = (r) | (g << 8) | (b << 16) | (a << 24);           ((Uint32*)win_surface->pixels)[(y * (win_surface->pitch / 4)) + x] = color;
    //         }
    //     }

    //     SDL_Texture* win_texture = SDL_CreateTextureFromSurface(rr->GetRenderer(), win_surface);
    //     SDL_SetTextureAlphaMod(win_texture, 0xFF);
    //     SDL_RenderCopy(rr->GetRenderer(), win_texture, NULL, NULL);

    //     SDL_FreeSurface(win_surface);
    //     SDL_DestroyTexture(win_texture);
    //     free(pixels);
    // }
}

void WorldManager::RenderDebugScreen(std::vector<std::string> debugStrings, Renderer* rr)
{
    float debugScale = 1;
    SDL_Rect textDimensions = rr->GetFont(Translations::GetJp())->GetTextDimensions();

    std::vector<int> debugWidths;
    for (size_t i = 0; i < debugStrings.size(); i++)
        debugWidths.push_back(rr->GetFont(Translations::GetJp())->GetTextDimensions(debugStrings[i].c_str(), debugScale).w + textDimensions.w * 4);

    int debug_w = *std::max_element(debugWidths.begin(), debugWidths.end());
    int debug_h = (debugStrings.size() + 2) * textDimensions.h * debugScale;

    SDL_Rect debugBg {0, 0, debug_w, debug_h};

    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 4, 4, 4, 0xA0);
    SDL_RenderFillRect(rr->GetRenderer(), &debugBg);

    // for (size_t i = 0; i < 11; i++)
    // {
    //     SDL_Rect perf = {
    //         (int)(textDimensions.w * 5 * debugScale),
    //         (int)(textDimensions.h * debugScale * (i+1 + 10)),
    //         (int)(frame_times[i] * 50),
    //         textDimensions.h
    //     };

    //     SDL_SetRenderDrawColor(rr->GetRenderer(), 0xC0, 0xC0, 0xC0, 0xA0);
    //     SDL_RenderFillRect(rr->GetRenderer(), &perf);
    // }

    for (size_t i = 0; i < debugStrings.size(); i++)
    {
        rr->RenderText(debugStrings[i].c_str(), textDimensions.w * debugScale, textDimensions.h * debugScale * (i+1), debugScale, false, Translations::GetJp());
    }
}

void WorldManager::CorrectOffset(SDL_Point mouse, float zoom_change)
{
    WorldManager::x_offset += ((WorldManager::x_offset - mouse.x) / WorldManager::zoom) * zoom_change;
    WorldManager::y_offset += ((WorldManager::y_offset - mouse.y) / WorldManager::zoom) * zoom_change;
}

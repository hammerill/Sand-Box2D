#include "WorldManager.h"

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
        WorldManager::deleteObject(i);
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

void WorldManager::addObject(BasePObj* obj) { WorldManager::order.push_back(obj); }
void WorldManager::deleteObject(int index)
{
    delete WorldManager::objects[index]; // Call a destructor.
    WorldManager::objects.erase(WorldManager::objects.begin() + index); // Remove from vector.
}

bool WorldManager::Step()
{
    if (WorldManager::speedCorrection)
        WorldManager::b = WorldManager::a;

    Ctrl::Check();

    if (Ctrl::getExit())
    {
        return false;
    }

    if (Ctrl::getFullscreen())
    {
        if (WorldManager::holdingFullscreenButton == false)
        {
            WorldManager::isFullscreen = !WorldManager::isFullscreen;

            WorldManager::goFullscreen(WorldManager::isFullscreen);
            
            WorldManager::holdingFullscreenButton = true;
        }        
    }
    else    
        WorldManager::holdingFullscreenButton = false;

    if (Ctrl::getDebug())
    {
        if (WorldManager::holdingDebugButton == false)
        {
            WorldManager::isDebug = !WorldManager::isDebug;            
            WorldManager::holdingDebugButton = true;
        }        
    }
    else    
        WorldManager::holdingDebugButton = false;

    if (Ctrl::getDeleteObjs())
    {
        for (int i = WorldManager::objects.size() - 1; i >= 0; i--)
        {
            if (WorldManager::objects[i]->isMarkedToDelete)
                WorldManager::deleteObject(i);        
        }
    }

    WorldManager::y_offset += Ctrl::getMoveUp() * WorldManager::move_speed;
    WorldManager::x_offset -= Ctrl::getMoveRight() * WorldManager::move_speed;
    WorldManager::y_offset -= Ctrl::getMoveDown() * WorldManager::move_speed;
    WorldManager::x_offset += Ctrl::getMoveLeft() * WorldManager::move_speed;

    if (Ctrl::getIsMoving())
    {
        WorldManager::x_offset += Ctrl::getDeltaX();
        WorldManager::y_offset += Ctrl::getDeltaY();
    }

    if (Ctrl::getIsPinching() && ((WorldManager::zoom + Ctrl::getDeltaPinch()) > 10))
    {
        correctOffset(  Ctrl::getMouse(),
                        Ctrl::getDeltaPinch());
        WorldManager::zoom += Ctrl::getDeltaPinch();
    }

    SDL_Point scr_center = {WorldManager::WINDOW_WIDTH / 2, WorldManager::WINDOW_HEIGHT / 2};

    if (WorldManager::zoom <= 1)
    {
        correctOffset(  Ctrl::getIsWheel() ? Ctrl::getMouse() : scr_center,
                        WorldManager::zoom - 1);
        WorldManager::zoom = 1;
    }
    else
    {
        correctOffset(  Ctrl::getIsWheel() ? Ctrl::getMouse() : scr_center,
                        Ctrl::getZoomOut() * WorldManager::zoom_speed * -1 * WorldManager::zoom);
        WorldManager::zoom -= Ctrl::getZoomOut() * WorldManager::zoom_speed * WorldManager::zoom;
    }

    if (WorldManager::zoom >= 1000)
    {
        correctOffset(  Ctrl::getIsWheel() ? Ctrl::getMouse() : scr_center,
                        1000 - WorldManager::zoom);
        WorldManager::zoom = 1000;
    }
    else
    {
        correctOffset(  Ctrl::getIsWheel() ? Ctrl::getMouse() : scr_center,
                        Ctrl::getZoomIn() * WorldManager::zoom_speed * WorldManager::zoom);
        WorldManager::zoom += Ctrl::getZoomIn() * WorldManager::zoom_speed * WorldManager::zoom;
    }

    WorldManager::world->Step(1.0f / 60.0f, 6, 2);

    if (Ctrl::getReset())
    {
        for (size_t i = 0; i < WorldManager::objects.size(); i++)
        {
            WorldManager::objects[i]->Reset();
        }
    }

    for (int i = WorldManager::order.size() - 1; i >= 0; i--)
    { // Load order.
        WorldManager::objects.push_back(WorldManager::order[i]);
        WorldManager::objects[WorldManager::objects.size() - 1]->Register(WorldManager::world, WorldManager::renderer);
        WorldManager::order.pop_back();        
    }

    for (size_t i = 0; i < WorldManager::objects.size(); i++)
    { // Out of bounds check.
        if (WorldManager::objects[i]->getBody()->GetPosition().x > 100 ||
            WorldManager::objects[i]->getBody()->GetPosition().y > 100 ||
            WorldManager::objects[i]->getBody()->GetPosition().x < -100 ||
            WorldManager::objects[i]->getBody()->GetPosition().y < -100 )
        {
            WorldManager::deleteObject(i);
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

    if (WorldManager::isDebug && Font::getLoaded())
    {
        SDL_Point mouse = Ctrl::getMouse();

        std::vector<std::string> debugStrings;

        debugStrings.push_back("DEBUG");
        debugStrings.push_back("");
        debugStrings.push_back("Camera offset X = " + std::to_string(WorldManager::x_offset));
        debugStrings.push_back("Camera offset Y = " + std::to_string(WorldManager::y_offset));
        debugStrings.push_back("Zoom = " + std::to_string(WorldManager::zoom));
        debugStrings.push_back("Mouse X = " + std::to_string(mouse.x));
        debugStrings.push_back("Mouse Y = " + std::to_string(mouse.y));
        debugStrings.push_back("Delta X = " + std::to_string(Ctrl::getDeltaX()));
        debugStrings.push_back("Delta Y = " + std::to_string(Ctrl::getDeltaY()));
        debugStrings.push_back("IsMoving? = " + std::to_string(Ctrl::getIsMoving()));
        debugStrings.push_back("Delta pinch = " + std::to_string(Ctrl::getDeltaPinch()));
        debugStrings.push_back("IsPinching? = " + std::to_string(Ctrl::getIsPinching()));
        debugStrings.push_back("Zoom In = " + std::to_string(Ctrl::getZoomIn()));
        debugStrings.push_back("Zoom Out = " + std::to_string(Ctrl::getZoomOut()));
        debugStrings.push_back("Objects count = " + std::to_string(WorldManager::world->GetBodyCount()));
        debugStrings.push_back("Objects rendered = " + std::to_string(renderedItemsCount));

        WorldManager::renderDebugScreen(debugStrings);        
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

SDL_Renderer* WorldManager::getRenderer() { return WorldManager::renderer; }

void WorldManager::goFullscreen(bool isToFullscreen)
{
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
    }
}

void WorldManager::renderDebugScreen(std::vector<std::string> debugStrings)
{
    const int fontWidth = 8; // Going to move it somewhere else later (or never)

    std::vector<int> debugWidths;
    for (size_t i = 0; i < debugStrings.size(); i++)
        debugWidths.push_back((debugStrings[i].size() + 2) * fontWidth);
    
    std::sort(debugWidths.begin(), debugWidths.end());

    int debug_w = (debugWidths[debugWidths.size()-1]);
    int debug_h = (debugStrings.size() + 2) * fontWidth;

    SDL_Rect debugBg {0, 0, debug_w, debug_h};

    SDL_SetRenderDrawBlendMode(WorldManager::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(WorldManager::renderer, 4, 4, 4, 0xA0);

    SDL_RenderFillRect(WorldManager::renderer, &debugBg);

    for (size_t i = 0; i < debugStrings.size(); i++)
    {
        Font::Render(WorldManager::renderer, debugStrings[i].c_str(), fontWidth, fontWidth * (i+1));
    }
}

void WorldManager::correctOffset(SDL_Point mouse, float zoom_change)
{
    WorldManager::x_offset += ((WorldManager::x_offset - mouse.x) / WorldManager::zoom) * zoom_change;
    WorldManager::y_offset += ((WorldManager::y_offset - mouse.y) / WorldManager::zoom) * zoom_change;
}

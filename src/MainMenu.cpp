#include "MainMenu.h"

SDL_Texture* logo;
SDL_Texture* title;

int zoom;

MainMenuPhysics::MainMenuPhysics() {}
MainMenuPhysics::~MainMenuPhysics() { MainMenuPhysics::FreeMemory(); }

float GetRandomFloat(float min, float max)
{
    return  min
            +
            static_cast<float> (rand())
            /
            (static_cast<float> (RAND_MAX / (max-min)));
}

void MainMenuPhysics::Init(float paddle_width)
{
    MainMenuPhysics::FreeMemory();
    MainMenuPhysics::world = new b2World({0, 9.81});

    MainMenuPhysics::RAD2DEG = 180 / M_PI;

    // PADDLE
    MainMenuPhysics::paddle_width = paddle_width;
    MainMenuPhysics::paddle_height = 0.45;
    
    MainMenuPhysics::SetPaddleDesiredPosition({1, 2});

    b2BodyDef paddle_def = b2BodyDef();
    b2PolygonShape paddle_shape = b2PolygonShape();
    b2FixtureDef paddle_fixture_def = b2FixtureDef();

    paddle_def.type = b2_kinematicBody;
    paddle_def.angle = 0.1;
    paddle_def.position.Set(1, 3);

    paddle_shape.SetAsBox(paddle_width / 2.0, paddle_height / 2.0);
    
    paddle_fixture_def.shape = &(paddle_shape);
    paddle_fixture_def.density = 1;
    paddle_fixture_def.friction = 0.3f;
    paddle_fixture_def.restitution = 0.5f;

    MainMenuPhysics::paddle = world->CreateBody(&(paddle_def));
    MainMenuPhysics::paddle->CreateFixture(&(paddle_fixture_def));
    /////////

    // BOX LOGO
    b2BodyDef box_logo_def = b2BodyDef();
    b2PolygonShape box_logo_shape = b2PolygonShape();
    b2FixtureDef box_logo_fixture_def = b2FixtureDef();

    box_logo_def.type = b2_dynamicBody;
    box_logo_def.angle = 0;
    box_logo_def.position.Set(0.5, 0.5);

    box_logo_shape.SetAsBox(0.5, 0.5);
    
    box_logo_fixture_def.shape = &(box_logo_shape);
    box_logo_fixture_def.density = 1;
    box_logo_fixture_def.friction = 0.3f;
    box_logo_fixture_def.restitution = 0.5f;

    MainMenuPhysics::box_logo = world->CreateBody(&(box_logo_def));
    MainMenuPhysics::box_logo->SetAngularVelocity(GetRandomFloat(-0.5, 0.5));
    MainMenuPhysics::box_logo->CreateFixture(&(box_logo_fixture_def));
    ///////////
}

const float paddle_speed = 5;
void MainMenuPhysics::Step()
{
    b2Vec2 paddle_pos = MainMenuPhysics::paddle->GetPosition();
    b2Vec2 paddle_vec = {
        (MainMenuPhysics::paddle_desired_pos.x - paddle_pos.x) * paddle_speed,
        (MainMenuPhysics::paddle_desired_pos.y - paddle_pos.y) * paddle_speed
    };
    MainMenuPhysics::paddle->SetLinearVelocity(paddle_vec);

    MainMenuPhysics::world->Step(1.0 / 60.0, 3, 3);
}
void MainMenuPhysics::RenderBG(Renderer* rr, int x_offset, int y_offset)
{
    
}
void MainMenuPhysics::RenderBox(Renderer* rr, int x_offset, int y_offset)
{
    b2Vec2 box_logo_pos = MainMenuPhysics::box_logo->GetPosition();
    SDL_Rect box_logo_rect;
    
    box_logo_rect.w = zoom;
    box_logo_rect.h = zoom;

    box_logo_rect.x = (box_logo_pos.x * zoom) + x_offset - (box_logo_rect.w / 2.0f);
    box_logo_rect.y = (box_logo_pos.y * zoom) + y_offset - (box_logo_rect.h / 2.0f);

    SDL_RenderCopyEx(rr->GetRenderer(), logo, NULL, &box_logo_rect, MainMenuPhysics::box_logo->GetAngle() * MainMenuPhysics::RAD2DEG, NULL, SDL_FLIP_NONE);
}
void MainMenuPhysics::RenderPaddle(Renderer* rr, int x_offset, int y_offset)
{
    b2Vec2 paddle_pos = MainMenuPhysics::paddle->GetPosition();
    SDL_Rect paddle_rect;
    
    paddle_rect.w = MainMenuPhysics::paddle_width * zoom;
    paddle_rect.h = MainMenuPhysics::paddle_height * zoom;

    paddle_rect.x = (paddle_pos.x * zoom) + x_offset - (paddle_rect.w / 2.0f);
    paddle_rect.y = (paddle_pos.y * zoom) + y_offset - (paddle_rect.h / 2.0f);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(rr->GetRenderer(), &paddle_rect);
}

void MainMenuPhysics::SetPaddleDesiredPosition(b2Vec2 pos)
{
    MainMenuPhysics::paddle_desired_pos = pos;
}
void MainMenuPhysics::SetPaddlePositionPermanently(b2Vec2 pos)
{
    MainMenuPhysics::paddle->SetTransform(pos, 0);
    MainMenuPhysics::paddle_desired_pos = pos;
}

void MainMenuPhysics::FreeMemory()
{
    if (MainMenuPhysics::paddle != nullptr)
    {
        MainMenuPhysics::world->DestroyBody(MainMenuPhysics::paddle);
        MainMenuPhysics::paddle = nullptr;
    }
    if (MainMenuPhysics::box_logo != nullptr)
    {
        MainMenuPhysics::world->DestroyBody(MainMenuPhysics::box_logo);
        MainMenuPhysics::box_logo = nullptr;
    }

    if (MainMenuPhysics::world != nullptr)
    {
        delete MainMenuPhysics::world;
        MainMenuPhysics::world = nullptr;
    }
}

MainMenu::MainMenu() {}
MainMenu::~MainMenu()
{
    SDL_DestroyTexture(logo);
    SDL_DestroyTexture(title);
}

const float paddle_width = 6.5; 
void MainMenu::Init()
{
    MainMenu::menu_items = std::vector<std::string>();
    MainMenu::hovered_item = 0;
    MainMenu::status = "";

    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_play"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_community"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_level_editor"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_settings"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_about"));
    MainMenu::menu_items.push_back(Translations::Load("menu.json/item_exit"));

    AnimationManager::InitAnim(ANIM_FADE_IN);

    MainMenu::physics.Init(paddle_width);
    
    logo = nullptr;  
    title = nullptr;    
}

SDL_Rect GetItemRect(Renderer* rr, std::vector<std::string> menu_items, size_t item_index)
{
    int menuScale = rr->GetWindowParams().height / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWindowParams().width / 2.5) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 1.5) - (menu_h / 2.25);

    SDL_Rect rect = rr->GetFont()->GetTextDimensions(menu_items[item_index].c_str(), menuScale);

    rect.x = x_offset - textDimensions.w / 16;
    rect.y = y_offset + textDimensions.h * distanceScale * (int)item_index - textDimensions.h / 16;
    rect.w = menu_w + textDimensions.w / 8;
    rect.h += textDimensions.h / 8;

    return rect;
}

SDL_Point GetPaddleCenterInPx(Renderer* rr, std::vector<std::string> menu_items, size_t item_index)
{
    int menuScale = rr->GetWindowParams().height / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWindowParams().width / 2.5) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 1.5) - (menu_h / 2.25);

    SDL_Point result = {
        x_offset + menu_w / 2,
        (int)(y_offset + textDimensions.h * distanceScale * item_index + textDimensions.h / 2)
    };

    int logo_length = 
        (rr->GetWindowParams().height / 6) 
        +
        (rr->GetWindowParams().height / 16)
        +
        (78 * menuScale * 3);
    
    int logo_height = y_offset - textDimensions.h / 16;

    SDL_Point physics_center_offset = {
        rr->GetWindowParams().width / 2 - logo_length / 2,
        logo_height / 2 - rr->GetWindowParams().height / 12
    };

    result.x -= physics_center_offset.x;
    result.y -= physics_center_offset.y;

    return result;
}

bool MainMenu::Step(Renderer* rr, Controls ctrl, Controls old_ctrl)
{
    if (logo == nullptr)
        logo = SDL_CreateTextureFromSurface(rr->GetRenderer(), IMG_Load("./assets/img/logo.png"));
    if (title == nullptr)
        title = SDL_CreateTextureFromSurface(rr->GetRenderer(), IMG_Load("./assets/img/title.png"));
    
    zoom = rr->GetWindowParams().height / 6;

    SDL_Point paddle_center_px = GetPaddleCenterInPx(rr, MainMenu::menu_items, MainMenu::hovered_item);
    b2Vec2 paddle_desired_pos = {
        paddle_center_px.x / (float)zoom,
        paddle_center_px.y / (float)zoom
    };

    // MainMenu::physics.Step(); // comment line below when realized
        MainMenu::physics.SetPaddlePositionPermanently(paddle_desired_pos);
    
    if (AnimationManager::StepAnim(ANIM_FADE))
    {
        MainMenu::physics.SetPaddlePositionPermanently(paddle_desired_pos);
        return true;
    }
    else
    {
        MainMenu::physics.SetPaddleDesiredPosition(paddle_desired_pos);
        if (MainMenu::status == "fadeout")
        {
            switch (MainMenu::hovered_item)
            {
            case 0:
                MainMenu::status = "play";
                MainMenu::level_name = "DEFAULT LEVEL";
                break;
            case 1:
                MainMenu::status = "community";
                break;
            case 2:
                MainMenu::status = "level_editor";
                break;
            case 3:
                MainMenu::status = "settings";
                break;
            case 4:
                MainMenu::status = "about";
                break;
            
            default:
                MainMenu::status = "";
                break;
            }
            return false; // False signifies that we've reached the end of MainMenu lifecycle.
            // From now on, its `will` will be stored only at "status" variable.
        }
    }

    if (ctrl.MenuEnter())
    {
        rr->GetSounds()->PlaySfx("menu_enter");
        MainMenu::status = "fadeout";
        AnimationManager::InitAnim(ANIM_FADE_OUT);
        return true;
    }

    if (rr->GetCursor(old_ctrl))
    {
        for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        {
            SDL_Point point = ctrl.GetMouse();
            SDL_Rect rect = GetItemRect(rr, MainMenu::menu_items, i);

            if (SDL_PointInRect(&point, &rect))
            {
                if (i != MainMenu::hovered_item)
                {
                    rr->GetSounds()->PlaySfx("menu_switch");
                    MainMenu::hovered_item = i;
                }
                if (old_ctrl.MousePress() && !ctrl.MousePress())
                {
                    rr->GetSounds()->PlaySfx("menu_enter");
                    MainMenu::status = "fadeout";
                    AnimationManager::InitAnim(ANIM_FADE_OUT);
                }
                return true;
            }
        }
    }
    
    if (ctrl.MenuUp() && !old_ctrl.MenuUp())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        MainMenu::hovered_item = MainMenu::hovered_item == 0
                                    ? MainMenu::menu_items.size() - 1
                                    : MainMenu::hovered_item - 1;
    }
    if (ctrl.MenuDown() && !old_ctrl.MenuDown())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        MainMenu::hovered_item = MainMenu::hovered_item == MainMenu::menu_items.size() - 1
                                    ? 0
                                    : MainMenu::hovered_item + 1;
    }
        
    return true;
}

void MainMenu::Render(Renderer* rr)
{
    bool hover_blinker = MainMenu::status == "fadeout" && (int)(rr->GetFrames() / 10) % 2;

    int menuScale = rr->GetWindowParams().height / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (MainMenu::menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWindowParams().width / 2.5) - (menu_w / 2);
    int y_offset = (rr->GetWindowParams().height / 1.5) - (menu_h / 2.25);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0x10, 0x10, 0x10, 0);
    SDL_RenderClear(rr->GetRenderer());

    int logo_length = 
        (rr->GetWindowParams().height / 6) 
        +
        (rr->GetWindowParams().height / 16)
        +
        (78 * menuScale * 3);
    
    int logo_height = y_offset - textDimensions.h / 16;

    SDL_Point physics_center_offset = {
        rr->GetWindowParams().width / 2 - logo_length / 2,
        logo_height / 2 - rr->GetWindowParams().height / 12
    };

    MainMenu::physics.RenderPaddle(rr, physics_center_offset.x, physics_center_offset.y);

    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
    {
        if (i == MainMenu::hovered_item && !hover_blinker)
        {
            // SDL_Rect hover_bg = rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale);

            // hover_bg.x = x_offset - textDimensions.w / 16;
            // hover_bg.y = y_offset + textDimensions.h * distanceScale * (int)i - textDimensions.h / 16;
            // hover_bg.w += textDimensions.w / 8;
            // hover_bg.h += textDimensions.h / 8;

            // SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            // SDL_RenderFillRect(rr->GetRenderer(), &hover_bg);

            rr->RenderText(
                MainMenu::menu_items[i].c_str(),
                x_offset,
                y_offset + textDimensions.h * distanceScale * i,
                menuScale,
                false,
                Translations::GetJp(), 0, 0, 0
            );
            // rr->RenderText(
            //     "▶",
            //     x_offset - textDimensions.w * distanceScale,
            //     y_offset + textDimensions.h * distanceScale * i,
            //     menuScale,
            //     false,
            //     true
            // );
        }
        else
            rr->RenderText(
                MainMenu::menu_items[i].c_str(),
                x_offset,
                y_offset + textDimensions.h * distanceScale * i,
                menuScale,
                false,
                Translations::GetJp()
            );
    }

    MainMenu::physics.RenderBox(rr, physics_center_offset.x, physics_center_offset.y);

    SDL_Rect title_rect = {
        rr->GetWindowParams().width / 2 - logo_length / 2 + (rr->GetWindowParams().height / 6) + (rr->GetWindowParams().height / 16),
        logo_height / 2 - (7 * menuScale * 3) / 2,
        78 * menuScale * 3,
        7 * menuScale * 3
    };
    SDL_RenderCopyEx(rr->GetRenderer(), title, NULL, &title_rect, 0, NULL, SDL_FLIP_NONE);

    const char* version_string = "v1.0.0 DEMO";
    SDL_Rect version_dimensions = rr->GetFont()->GetTextDimensions(version_string, menuScale / 2);
    rr->RenderText(
        version_string,
        rr->GetWindowParams().width - version_dimensions.w,
        rr->GetWindowParams().height - version_dimensions.h,
        menuScale / 2,
        false, false,
        0x40, 0x40, 0x40
    );
    
    AnimationManager::RenderAnim(ANIM_FADE, rr);
}

std::string MainMenu::GetStatus()   { return MainMenu::status; }

std::string MainMenu::GetLevelName()    { return MainMenu::level_name; }

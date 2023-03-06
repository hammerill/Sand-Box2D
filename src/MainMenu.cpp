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

const float paddle_slope = 0;

void MainMenuPhysics::Init()
{
    MainMenuPhysics::FreeMemory();
    MainMenuPhysics::world = new b2World({0, 9.81});

    MainMenuPhysics::RAD2DEG = 180 / M_PI;

    MainMenuPhysics::paddle_inited = false;

    MainMenuPhysics::SetTitleAlphaPermanently(0xFF);

    // BOX LOGO
    MainMenuPhysics::box_active = false;

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

void MainMenuPhysics::InitPaddle(float paddle_width)
{
    if (MainMenuPhysics::paddle_inited)
        return;
    // PADDLE
    MainMenuPhysics::paddle_width = paddle_width;
    MainMenuPhysics::paddle_height = 0.45;
    
    MainMenuPhysics::SetPaddleDesiredPosition({1, 2});

    b2BodyDef paddle_def = b2BodyDef();
    b2PolygonShape paddle_shape = b2PolygonShape();
    b2FixtureDef paddle_fixture_def = b2FixtureDef();

    paddle_def.type = b2_kinematicBody;
    paddle_def.angle = paddle_slope;
    paddle_def.position.Set(1, 3);

    paddle_shape.SetAsBox(paddle_width / 2.0, paddle_height / 2.0);
    
    paddle_fixture_def.shape = &(paddle_shape);
    paddle_fixture_def.density = 1;
    paddle_fixture_def.friction = 0.3f;
    paddle_fixture_def.restitution = 0.5f;

    MainMenuPhysics::paddle = world->CreateBody(&(paddle_def));
    MainMenuPhysics::paddle->CreateFixture(&(paddle_fixture_def));
    /////////
    MainMenuPhysics::paddle_inited = true;
}

const float paddle_speed = 7.5;
const float title_speed = 0.1;

void MainMenuPhysics::Step()
{
    MainMenuPhysics::title_factual_alpha += (MainMenuPhysics::title_desired_alpha - MainMenuPhysics::title_factual_alpha) * title_speed;

    if (MainMenuPhysics::paddle_inited)
    {
        b2Vec2 paddle_pos = MainMenuPhysics::paddle->GetPosition();
        b2Vec2 paddle_vec = {
            (MainMenuPhysics::paddle_desired_pos.x - paddle_pos.x) * paddle_speed,
            (MainMenuPhysics::paddle_desired_pos.y - paddle_pos.y) * paddle_speed
        };
        MainMenuPhysics::paddle->SetLinearVelocity(paddle_vec);
    }

    if (MainMenuPhysics::box_active)
    {
        MainMenuPhysics::box_logo->SetGravityScale(1);

        auto pos = MainMenuPhysics::box_logo->GetPosition();
        if (abs(pos.x) > 10 || abs(pos.y) > 10)
        {
            MainMenuPhysics::box_active = false;
            MainMenuPhysics::SetTitleDesiredAlpha(0xFF);
        }
    }
    else
    {
        MainMenuPhysics::box_logo->SetGravityScale(0);
        MainMenuPhysics::box_logo->SetAngularVelocity(0);
        MainMenuPhysics::box_logo->SetLinearVelocity({0, 0});
        MainMenuPhysics::box_logo->SetTransform({0.5, 0.5}, 0);
    }

    MainMenuPhysics::world->Step(1.0 / 60.0, 2, 1);
}
void MainMenuPhysics::RenderTitle(Renderer* rr, SDL_Rect& title_rect)
{
    SDL_SetTextureAlphaMod(title, MainMenuPhysics::title_factual_alpha);
    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(rr->GetRenderer(), title, NULL, &title_rect);
}
void MainMenuPhysics::RenderBox(Renderer* rr, int x_offset, int y_offset)
{
    uint8_t box_alpha = MainMenuPhysics::box_active ? 0xFF : MainMenuPhysics::title_factual_alpha;

    b2Vec2 box_logo_pos = MainMenuPhysics::box_logo->GetPosition();
    SDL_Rect box_logo_rect;
    
    box_logo_rect.w = zoom;
    box_logo_rect.h = zoom;

    box_logo_rect.x = (box_logo_pos.x * zoom) + x_offset - (box_logo_rect.w / 2.0f);
    box_logo_rect.y = (box_logo_pos.y * zoom) + y_offset - (box_logo_rect.h / 2.0f);

    SDL_SetTextureAlphaMod(logo, box_alpha);
    SDL_SetTextureBlendMode(logo, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(rr->GetRenderer(), SDL_BLENDMODE_BLEND);

    SDL_RenderCopyEx(
        rr->GetRenderer(),
        logo,
        NULL,
        &box_logo_rect,
        MainMenuPhysics::box_logo->GetAngle() * MainMenuPhysics::RAD2DEG,
        NULL,
        SDL_FLIP_NONE
    );
}
void MainMenuPhysics::RenderPaddle(Renderer* rr, int x_offset, int y_offset, float menu_scale)
{
    if (!MainMenuPhysics::paddle_inited)
        return;

    b2Vec2 paddle_pos = MainMenuPhysics::paddle->GetPosition();
    SDL_Rect paddle_rect;
    
    paddle_rect.w = MainMenuPhysics::paddle_width * zoom;
    paddle_rect.h = MainMenuPhysics::paddle_height * zoom;

    paddle_rect.x = (paddle_pos.x * zoom) + x_offset - (paddle_rect.w / 2.0f);
    paddle_rect.y = (paddle_pos.y * zoom) + y_offset - (paddle_rect.h / 2.0f);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(rr->GetRenderer(), &paddle_rect);

    rr->RenderText(
        "▶",
        paddle_rect.x - (paddle_rect.h / 2),
        paddle_rect.y + (paddle_rect.h / 2),
        menu_scale,
        true, true
    );
}

SDL_Rect MainMenuPhysics::GetPaddleRect(Renderer* rr, int x_offset, int y_offset, float menu_scale)
{
    if (!MainMenuPhysics::paddle_inited)
        return {};

    b2Vec2 paddle_pos = MainMenuPhysics::paddle->GetPosition();
    SDL_Rect paddle_rect;
    
    paddle_rect.w = MainMenuPhysics::paddle_width * zoom;
    paddle_rect.h = MainMenuPhysics::paddle_height * zoom;

    paddle_rect.x = (paddle_pos.x * zoom) + x_offset - (paddle_rect.w / 2.0f);
    paddle_rect.y = (paddle_pos.y * zoom) + y_offset - (paddle_rect.h / 2.0f);

    return paddle_rect;
}

void MainMenuPhysics::ActivateBox(Renderer* rr)
{
    if (MainMenuPhysics::box_active)
        return;
    
    rr->GetSounds()->PlaySfx("menu_hit");
    MainMenuPhysics::box_logo->SetAngularVelocity(GetRandomFloat(-0.5, 0.5));
    MainMenuPhysics::box_active = true;

    MainMenuPhysics::SetTitleDesiredAlpha(0);
}
SDL_Rect MainMenuPhysics::GetBoxRect(Renderer* rr, int x_offset, int y_offset)
{
    b2Vec2 box_logo_pos = MainMenuPhysics::box_logo->GetPosition();
    SDL_Rect box_logo_rect;
    
    box_logo_rect.w = zoom;
    box_logo_rect.h = zoom;

    box_logo_rect.x = (box_logo_pos.x * zoom) + x_offset - (box_logo_rect.w / 2.0f);
    box_logo_rect.y = (box_logo_pos.y * zoom) + y_offset - (box_logo_rect.h / 2.0f);

    return box_logo_rect;
}

void MainMenuPhysics::SetPaddleDesiredPosition(b2Vec2 pos)
{
    MainMenuPhysics::paddle_desired_pos = pos;
}
void MainMenuPhysics::SetPaddlePositionPermanently(b2Vec2 pos)
{
    if (!MainMenuPhysics::paddle_inited)
        return;
    
    MainMenuPhysics::paddle->SetTransform(pos, paddle_slope);
    MainMenuPhysics::paddle_desired_pos = pos;
}

void MainMenuPhysics::SetTitleDesiredAlpha(uint8_t alpha)
{
    MainMenuPhysics::title_desired_alpha = alpha;
}
void MainMenuPhysics::SetTitleAlphaPermanently(uint8_t alpha)
{
    MainMenuPhysics::title_factual_alpha = alpha;
    MainMenuPhysics::title_desired_alpha = alpha;
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

    SDL_DestroyTexture(temp);
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

    MainMenu::physics.Init();

    MainMenu::temp = nullptr;
    
    logo = nullptr;  
    title = nullptr;    
}

SDL_Rect GetItemRect(Renderer* rr, std::vector<std::string> menu_items, size_t item_index)
{
    int menuScale = rr->GetHeight() / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWidth() / 2.9) - (menu_w / 2);
    int y_offset = (rr->GetHeight() / 1.5) - (menu_h / 2.25);

    SDL_Rect rect = rr->GetFont()->GetTextDimensions(menu_items[item_index].c_str(), menuScale);

    rect.x = x_offset - textDimensions.w / 16;
    rect.y = y_offset + textDimensions.h * distanceScale * (int)item_index - textDimensions.h / 16;
    rect.w = menu_w + textDimensions.w / 8;
    rect.h += textDimensions.h / 8;

    return rect;
}

SDL_Point GetPaddleCenterInPx(Renderer* rr, std::vector<std::string> menu_items, size_t item_index)
{
    int menuScale = rr->GetHeight() / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWidth() / 2.9) - (menu_w / 2);
    int y_offset = (rr->GetHeight() / 1.5) - (menu_h / 2.25);

    SDL_Point result = {
        x_offset + menu_w / 2,
        (int)(y_offset + textDimensions.h * distanceScale * item_index + textDimensions.h / 2)
    };

    int logo_length = 
        (rr->GetHeight() / 6) 
        +
        (rr->GetHeight() / 16)
        +
        (78 * menuScale * 3);
    
    int logo_height = y_offset - textDimensions.h / 16;

    SDL_Point physics_center_offset = {
        rr->GetWidth() / 2 - logo_length / 2,
        logo_height / 2 - rr->GetHeight() / 12
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
    
    zoom = rr->GetHeight() / 6;

    SDL_Point paddle_center_px = GetPaddleCenterInPx(rr, MainMenu::menu_items, MainMenu::hovered_item);
    b2Vec2 paddle_desired_pos = {
        paddle_center_px.x / (float)zoom,
        paddle_center_px.y / (float)zoom
    };

    MainMenu::physics.Step();
    
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
                MainMenu::level_name = Translations::Load("levels.json/default_level_name");
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

    if (rr->GetCursor(ctrl) || rr->GetCursor(old_ctrl))
    {
        int menuScale = rr->GetHeight() / 250;
        SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

        float distanceScale = 1.2;

        std::vector<int> menuWidths;
        for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
            menuWidths.push_back(rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale).w);

        int menu_h = (MainMenu::menu_items.size()) * textDimensions.h * distanceScale;

        int y_offset = (rr->GetHeight() / 1.5) - (menu_h / 2.25);

        int logo_length = 
            (rr->GetHeight() / 6) 
            +
            (rr->GetHeight() / 16)
            +
            (78 * menuScale * 3);
        
        int logo_height = y_offset - textDimensions.h / 16;

        SDL_Point physics_center_offset = {
            rr->GetWidth() / 2 - logo_length / 2,
            logo_height / 2 - rr->GetHeight() / 12
        };

        SDL_Point mouse = ctrl.GetMouse();
        SDL_Rect box = MainMenu::physics.GetBoxRect(rr, physics_center_offset.x, physics_center_offset.y);

        if (ctrl.MenuMouse() && !old_ctrl.MenuMouse() && SDL_PointInRect(&mouse, &box))
            MainMenu::physics.ActivateBox(rr);

        for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        {
            SDL_Rect rect = GetItemRect(rr, MainMenu::menu_items, i);

            if (SDL_PointInRect(&mouse, &rect))
            {
                if (i != MainMenu::hovered_item)
                {
                    rr->GetSounds()->PlaySfx("menu_switch");
                    MainMenu::hovered_item = i;
                }
                if (old_ctrl.MenuMouse() && !ctrl.MenuMouse())
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

void RenderBlackText(Renderer* rr, const char *text, int x, int y, float scale, SDL_Rect paddleRect, Font* font)
{
    SDL_Rect textRect = font->GetTextDimensions(text, scale);
    textRect.x = x;
    textRect.y = y;

    SDL_Rect dstRect;
    if (SDL_IntersectRect(&paddleRect, &textRect, &dstRect) == SDL_FALSE)
        return;

    SDL_Rect srcRect = {
        textRect.x - x, textRect.y - y,
        dstRect.w, dstRect.h
    };

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font->GetFont(), text, {0, 0, 0});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rr->GetRenderer(), textSurface);

    SDL_RenderCopy(rr->GetRenderer(), textTexture, &srcRect, &dstRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void MainMenu::Render(Renderer* rr)
{
    bool hover_blinker = MainMenu::status == "fadeout" && (int)(rr->GetFrames() / 10) % 2;

    int menuScale = rr->GetHeight() / 250;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", menuScale);

    float distanceScale = 1.2;

    std::vector<int> menuWidths;
    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        menuWidths.push_back(rr->GetFont()->GetTextDimensions(MainMenu::menu_items[i].c_str(), menuScale).w);

    int menu_w = *std::max_element(menuWidths.begin(), menuWidths.end());
    int menu_h = (MainMenu::menu_items.size()) * textDimensions.h * distanceScale;

    int x_offset = (rr->GetWidth() / 2.9) - (menu_w / 2);
    int y_offset = (rr->GetHeight() / 1.5) - (menu_h / 2.25);

    int logo_length = 
        (rr->GetHeight() / 6) 
        +
        (rr->GetHeight() / 16)
        +
        (78 * menuScale * 3);
    
    int logo_height = y_offset - textDimensions.h / 16;

    SDL_Point physics_center_offset = {
        rr->GetWidth() / 2 - logo_length / 2,
        logo_height / 2 - rr->GetHeight() / 12
    };

    // That's stupid. It's a logical call in the render function. Should be reformatted.
    // (Also ignore that I do a lot of graphical stuff at step (logical) function)
    MainMenu::physics.InitPaddle(((float)(menu_w + textDimensions.w) / (float)zoom));

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0x10, 0x10, 0x10, 0xFF);
    SDL_RenderClear(rr->GetRenderer());
    
    for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
    {
        rr->RenderText(
            MainMenu::menu_items[i].c_str(),
            x_offset,
            y_offset + textDimensions.h * distanceScale * i,
            menuScale,
            false,
            Translations::GetJp()
        );
    }

    MainMenu::physics.RenderPaddle(rr, physics_center_offset.x, physics_center_offset.y, menuScale);

    if (!hover_blinker)
        for (size_t i = 0; i < MainMenu::menu_items.size(); i++)
        {
            RenderBlackText(
                rr,
                MainMenu::menu_items[i].c_str(),
                x_offset,
                y_offset + textDimensions.h * distanceScale * i,
                menuScale,
                MainMenu::physics.GetPaddleRect(rr, physics_center_offset.x, physics_center_offset.y, menuScale),
                rr->GetFont(Translations::GetJp())
            );
        }


    SDL_Rect title_rect = {
        rr->GetWidth() / 2 - logo_length / 2 + (rr->GetHeight() / 6) + (rr->GetHeight() / 16),
        logo_height / 2 - (7 * menuScale * 3) / 2,
        78 * menuScale * 3,
        7 * menuScale * 3
    };
    MainMenu::physics.RenderTitle(rr, title_rect);

    const char* version_string = "v1.0.0 DEMO";
    SDL_Rect version_dimensions = rr->GetFont()->GetTextDimensions(version_string, menuScale / 2);
    rr->RenderText(
        version_string,
        rr->GetWidth() - version_dimensions.w,
        rr->GetHeight() - version_dimensions.h,
        menuScale / 2,
        false, false,
        0x40, 0x40, 0x40
    );

    MainMenu::physics.RenderBox(rr, physics_center_offset.x, physics_center_offset.y);

    AnimationManager::RenderAnim(ANIM_FADE, rr);
}

std::string MainMenu::GetStatus()       { return MainMenu::status; }
std::string MainMenu::GetLevelName()    { return MainMenu::level_name; }

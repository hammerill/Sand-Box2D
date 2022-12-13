#include "BasePObj.h"

b2Body* BasePObj::GetBody()
{
    return BasePObj::body;
}
int BasePObj::GetId()
{
    return BasePObj::id;
}

SDL_Texture* BasePObj::LoadTexture(std::map<std::string, SDL_Texture*> textures, std::string path, SDL_Renderer* renderer)
{
    if (textures.count(path) == 0)
    {
        auto texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(path.c_str()));
        textures[path] = texture != NULL ? texture : nullptr;
    }
    return textures[path];
}

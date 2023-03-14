#include "SoundManager.h"

SoundManager::SoundManager()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

SoundManager::~SoundManager()
{
    // Unload all the sounds.
    for (std::map<std::string, Mix_Chunk*>::iterator itr = SoundManager::sounds.begin(); itr != SoundManager::sounds.end(); itr++)
    {
        Mix_FreeChunk(itr->second);
    }
    SoundManager::sounds.clear();

    Mix_CloseAudio();
    Mix_Quit();
}

SoundManager::SoundManager(std::string path_to_sfx_base)
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    SoundManager::LoadSfx(path_to_sfx_base);
}

void SoundManager::LoadSfx(std::string path_to_sfx_base)
{
    // Load Index JSON
    Json::Value index;
    std::ifstream ifs(path_to_sfx_base + "/index.json");
    Json::Reader reader;
    reader.parse(ifs, index);
    //////////////////

    // Unload SFX if they're here
    for (std::map<std::string, Mix_Chunk*>::iterator itr = SoundManager::sounds.begin(); itr != SoundManager::sounds.end(); itr++)
    {
        Mix_FreeChunk(itr->second);
    }
    SoundManager::sounds.clear();
    /////////////////////////////

    for (Json::Value::const_iterator itr = index.begin() ; itr != index.end() ; itr++)
    {
        std::string temp = (path_to_sfx_base + "/" + itr->asString());
        Mix_Chunk* temp_sound = Mix_LoadWAV(temp.c_str());

        if (temp_sound != NULL)
            SoundManager::sounds[itr.key().asString()] = temp_sound;
    }
}

void SoundManager::PlaySfx(std::string sound_name)
{
    if (SoundManager::sounds.count(sound_name))
        Mix_PlayChannel(-1, SoundManager::sounds[sound_name], 0);
}

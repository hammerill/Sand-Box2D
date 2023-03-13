#pragma once

#include <SDL2/SDL_mixer.h>

#include <map>
#include <string>
#include <fstream>

#include "Json.h"

/// @brief Class for managing sound effects (and maybe music in future).
class SoundManager
{
private:
    std::map<std::string, Mix_Chunk*> sounds = std::map<std::string, Mix_Chunk*>();
public:
    SoundManager();
    ~SoundManager();

    /// @brief Will try to load all the sound effects.
    /// @param path_to_sfx_base path to the directory where all the sound effects are located. Should
    /// contain index.json which looks like this: {"big_sound_name": "PathTo/Big/Sound.wav", "...": "..."}
    SoundManager(std::string path_to_sfx_base);

    /// @brief Will try to load all the sound effects.
    /// @param path_to_sfx_base path to the directory where all the sound effects are located. Should
    /// contain index.json which looks like this: {"big_sound_name": "PathTo/Big/Sound.wav", "...": "..."}
    void LoadSfx(std::string path_to_sfx_base);

    /// @brief Will try to play a sound.
    /// @param sound_name sound name declared in the index.json
    void PlaySfx(std::string sound_name);
};

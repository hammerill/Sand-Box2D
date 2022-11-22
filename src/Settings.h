#pragma once

#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>

/// @brief Class that manages settings in the game and operates with theirs file.
class Settings
{
private:
    Json::Value settings;
    Json::Value def_settings;

    const char* path_to_settings = nullptr;
public:
    /// @brief Load settings file and create Settings instance.
    /// At every Set() call will write updated parameter to file if it's successfully loaded.
    /// Will also call Set() when setting default value while calling Get() and no value set.s
    /// @param path_to_settings path to the JSON where user settings are stored.
    /// If doesn't exist - it'll create it. If leave nullptr, settings affect only current session.
    /// @param path_to_def_settings path to the JSON where default settings are stored.
    /// If leave nullptr, all default settings will be considered as 0.
    Settings(const char* path_to_settings = nullptr, const char* path_to_def_settings = nullptr);

    /// @brief Set some value at the settings. Will affect current session and write it to the file if it's loaded.
    /// @param key Name of the parameter to be setted.
    /// @param value Value of the parameter to be setted.
    void Set(std::string key, Json::Value value);

    /// @brief Get some value from the settings.
    /// @param key Name of the parameter to be returned.
    /// @return Value of the parameter. If not set - set to default value and return it.
    Json::Value Get(std::string key);
};
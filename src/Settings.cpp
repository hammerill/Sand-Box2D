#include "Settings.h"

Settings::Settings(std::string base, std::string path_to_settings, std::string path_to_def_settings)
{
    Settings::settings = Json::Value();
    Settings::def_settings = Json::Value();

    if (path_to_settings != "")
    {
        Settings::path_to_settings = (base + "/" +path_to_settings).c_str();

        std::ifstream ifs(Settings::path_to_settings);
        Json::Reader reader;
        reader.parse(ifs, Settings::settings);
    }
    if (path_to_def_settings != "")
    {
        std::ifstream ifs(path_to_def_settings);
        Json::Reader reader;
        reader.parse(ifs, Settings::def_settings);
    }
}

void Settings::Set(std::string key, Json::Value value)
{
    Settings::settings[key] = value;

    if (Settings::path_to_settings != nullptr)
    {
        std::ofstream ofs;
        ofs.open(Settings::path_to_settings);

        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "    ";

        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(Settings::settings, &ofs);

        ofs.close();
    }
}

Json::Value Settings::Get(std::string key)
{
    if (!Settings::settings.isMember(key))
    {
        if (Settings::def_settings.isMember(key))
            Settings::Set(key, Settings::def_settings[key]);
        else
            Settings::Set(key, Json::Value(0));
    }

    return Settings::settings[key];
}

#include "Translations.h"

std::string Translations::translations_base = "", Translations::translation_code = "";
bool Translations::isJp = false;

Translations::Translations() {}
Translations::~Translations() {}

void Translations::LoadTranslation(std::string translations_base, std::string translation_code)
{
    Translations::translations_base = translations_base;
    Translations::translation_code = translation_code;
    Translations::isJp = Translations::translation_code == "jp";
}

std::string Translations::Load(std::string to_load)
{
    if (Translations::translations_base == "" || Translations::translation_code == "")
        return "";
    
    std::string path_to_json;
    Json::Value loaded_json;
    size_t slash_ptr = to_load.find_last_of("\\/");    

    if (slash_ptr == std::string::npos)
        return "";
    
    path_to_json =  Translations::translations_base
            + "/" + Translations::translation_code
            + "/" + to_load.substr(0, slash_ptr);

    std::ifstream ifs(path_to_json);
    Json::Reader reader;
    reader.parse(ifs, loaded_json);

    return loaded_json[to_load.substr(slash_ptr + 1)].asString();
}

bool Translations::GetJp()  { return Translations::isJp; }

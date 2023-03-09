#include "Files.h"

Files::Files() {}
Files::~Files() {}

#ifdef Vita
bool doesDirExist(const char* path) { 
	SceUID dir = sceIoDopen(path); 
 	if (dir >= 0)
    { 
 		sceIoDclose(dir); 
 		return 1; 
 	}
    else
 		return 0; 
} 
bool Files::MakeDirs(std::string path)
{
    if(!doesDirExist(path.c_str()))
    {
        int res = sceIoMkdir(path.c_str(), 0777);

        if (res == 0)
            return true;
        else
        {
            auto pos = path.find_last_of("\\/");
            if (pos == std::string::npos)
                return false;
            
            if (!Files::MakeDirs(path.substr(0, pos)))
                return false;
        }

        res = sceIoMkdir(path.c_str(), 0777);
        
        if (res == 0)
            return doesDirExist(path.c_str());
        else
            return false;
    }
    
    return true;
}

#else

bool Files::MakeDirs(std::string path)
{
    return std::filesystem::create_directories(path.c_str());
}

#endif

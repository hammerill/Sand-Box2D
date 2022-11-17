#include "NetworkManager.h"

CURL* NetworkManager::curl;
CURLcode NetworkManager::res;

std::string NetworkManager::repo;

#ifdef Vita

int file;

void openFile(std::string path)
{
    file = sceIoOpen(path.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
}
void closeFile()
{
    sceIoClose(file);
}
int* getFile()
{
    return &file;
}

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
bool mkDirs(std::string path)
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
            
            if (!mkDirs(path.substr(0, pos)))
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

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return sceIoWrite(*(int*) stream, ptr, size*nmemb);
}

#else

FILE* file;

void openFile(std::string path)
{
    file = fopen(path.c_str(), "wb");
}
void closeFile()
{
    fclose(file);
}
FILE* getFile()
{
    return file;
}

bool mkDirs(std::string path)
{
    return std::filesystem::create_directories(path.c_str());
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

#endif

NetworkManager::NetworkManager()
{
    curl_global_init(CURL_GLOBAL_ALL);
}
NetworkManager::~NetworkManager()
{
    curl_global_cleanup();
}

void NetworkManager::setRepo(std::string repo)
{
    NetworkManager::repo = repo;
}
CURLcode NetworkManager::DownloadFile(std::string base, std::string filepath)
{
    NetworkManager::curl = curl_easy_init();

    mkDirs(base);

    if (filepath.find_last_of("\\/") != std::string::npos)
    {
        mkDirs(base + "/" + filepath.substr(0, filepath.find_last_of("\\/")));
    }
    openFile(base + "/" + filepath);

    curl_easy_setopt(NetworkManager::curl, CURLOPT_URL, (NetworkManager::repo + "/" + filepath).c_str());
    curl_easy_setopt(NetworkManager::curl, CURLOPT_WRITEDATA, getFile());
    curl_easy_setopt(NetworkManager::curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(NetworkManager::curl, CURLOPT_SSL_VERIFYPEER, 0L);

    NetworkManager::res = curl_easy_perform(NetworkManager::curl);

    curl_easy_cleanup(NetworkManager::curl);
    closeFile();

    return NetworkManager::res;
}

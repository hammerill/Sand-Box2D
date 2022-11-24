#include "Network.h"

CURL* Network::curl;
CURLcode Network::res;

std::string Network::repo;

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
size_t writeData(void* ptr, size_t size, size_t nmemb, void* stream)
{
	return sceIoWrite(*(int*)stream, ptr, size * nmemb);
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
size_t writeData(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

#endif

Network::Network()
{
    curl_global_init(CURL_GLOBAL_ALL);
}
Network::~Network()
{
    curl_global_cleanup();
}

void Network::SetRepo(std::string repo)
{
    Network::repo = repo;
}
CURLcode Network::DownloadFile(std::string base, std::string filepath)
{
    Network::curl = curl_easy_init();

    Files::MakeDirs(base);

    if (filepath.find_last_of("\\/") != std::string::npos)
    {
        Files::MakeDirs(base + "/" + filepath.substr(0, filepath.find_last_of("\\/")));
    }
    openFile(base + "/" + filepath);

    curl_easy_setopt(Network::curl, CURLOPT_URL, (Network::repo + "/" + filepath).c_str());
    curl_easy_setopt(Network::curl, CURLOPT_WRITEDATA, getFile());
    curl_easy_setopt(Network::curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(Network::curl, CURLOPT_SSL_VERIFYPEER, 0L);

    Network::res = curl_easy_perform(Network::curl);

    curl_easy_cleanup(Network::curl);
    closeFile();

    return Network::res;
}

#include "NetworkManager.h"

CURL* NetworkManager::curl;
FILE* NetworkManager::file;
CURLcode NetworkManager::res;

std::string NetworkManager::repo;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

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

    std::filesystem::create_directories(base);

    if (filepath.find("\\/") != std::string::npos)
    {
        std::filesystem::create_directories((base + "/" + filepath.substr(0, filepath.find_last_of("\\/"))).c_str());
    }
    NetworkManager::file = fopen((base + "/" + filepath).c_str(), "wb");

    curl_easy_setopt(NetworkManager::curl, CURLOPT_URL, (NetworkManager::repo + "/" + filepath).c_str());
    curl_easy_setopt(NetworkManager::curl, CURLOPT_WRITEDATA, NetworkManager::file);
    curl_easy_setopt(NetworkManager::curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(NetworkManager::curl, CURLOPT_SSL_VERIFYPEER, 0L);

    NetworkManager::res = curl_easy_perform(NetworkManager::curl);

    curl_easy_cleanup(NetworkManager::curl);
    fclose(NetworkManager::file);

    return NetworkManager::res;
}


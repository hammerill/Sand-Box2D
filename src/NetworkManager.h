#pragma once

#include <curl/curl.h>

#include <filesystem>
#include <stdio.h>
#include <string>

/// @brief Static class for downloading files.
class NetworkManager
{
private:
    static CURL* curl;
    static FILE* file;
    static CURLcode res;

    static std::string repo;

public:
    NetworkManager();
    ~NetworkManager();

    /// @brief Set "repo".
    /// @param repo URL which targets us to rawgithub where levels are containing.
    static void setRepo(std::string repo);

    /// @brief Download file.
    /// @param base where is directory of all the levels stored on the platform are located?
    /// Will download file there.
    /// @param filepath path to the file to be downloaded, relatively to the "repo" (see setRepo()).
    /// @return response code (CURLcode).
    static CURLcode DownloadFile(std::string base, std::string filepath);
};


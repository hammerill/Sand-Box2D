#pragma once

#include <curl/curl.h>

#include "Files.h"

/// @brief Static class for downloading files.
class Network
{
private:
    static CURL* curl;
    static CURLcode res;

    static std::string repo;

public:
    Network();
    ~Network();

    /// @brief Set levels repo site.
    /// @param repo URL which targets us to rawgithub where levels are containing.
    static void SetRepo(std::string repo);

    /// @brief Download a file.
    /// @param base where is base data directory located? Will download file there.
    /// @param filepath path to the file to be downloaded, relatively to the "repo" (see SetRepo()) on URL
    /// and to "base" on file in local platform.
    /// @return response code (CURLcode).
    static CURLcode DownloadFile(std::string base, std::string filepath);
};


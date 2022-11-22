#pragma once

#include <curl/curl.h>

#ifdef Vita
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include "psp2/kernel/clib.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <string>
#else
#include <filesystem>
#include <stdio.h>
#include <string>
#endif

/// @brief Static class for downloading files.
class NetworkManager
{
private:
    static CURL* curl;
    static CURLcode res;

    static std::string repo;

public:
    NetworkManager();
    ~NetworkManager();

    /// @brief Set "repo".
    /// @param repo URL which targets us to rawgithub where levels are containing.
    static void SetRepo(std::string repo);

    /// @brief Download file.
    /// @param base where is directory of all the levels stored on the platform are located?
    /// Will download file there.
    /// @param filepath path to the file to be downloaded, relatively to the "repo" (see SetRepo()).
    /// @return response code (CURLcode).
    static CURLcode DownloadFile(std::string base, std::string filepath);
};


#pragma once

#ifdef Vita
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/clib.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <string>

#else
#include <filesystem>
#include <stdio.h>
#include <string>

#endif

/// @brief Static class for operating with filesystem.
class Files
{
private:
public:
    Files();
    ~Files();

    /// @brief Make a directory recursively.
    /// @param path directory to be created.
    /// @return true if success.
    static bool MakeDirs(std::string path);
};

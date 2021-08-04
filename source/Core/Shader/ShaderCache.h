#pragma once

#include <string>

bool InitShaderCache(const std::string libraryDir, const std::string cacheDir);

std::string GetShaderLibraryDir();
std::string GetShaderCacheDir();

#include "pch.h"
#include "Core/Shader/ShaderCache.h"

std::string CacheDir;
std::string LibraryDir;

bool InitShaderCache(const std::string libraryDir,const std::string cacheDir)
{
	LibraryDir	= libraryDir;
	CacheDir	= cacheDir;
	return true;
}

std::string GetShaderCacheDir()
{
	return CacheDir;
}

std::string GetShaderLibraryDir()
{
	return LibraryDir;
}


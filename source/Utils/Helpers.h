#pragma once
#include <filesystem>
#include "Core/typedefs.h"



std::vector<std::string> GetFileListing(std::string path, std::string file = "*", std::string extensions = "*");

#define HASH_SEED 2166136261

size_t Hash(const void* ptr, size_t size, size_t result = HASH_SEED);
size_t HashString(const char* str, size_t result = HASH_SEED);
size_t HashString(const std::string& str, size_t result = HASH_SEED);
size_t HashInt(const int type, size_t result = HASH_SEED);
size_t HashFloat(const float type, size_t result = HASH_SEED);
size_t HashPtr(const void* type, size_t result = HASH_SEED);

#ifdef _WIN32
const char* ReadTextFileA(const char* filename);
std::string ReadTextFileA(std::string);

std::vector<char> ReadBinaryFileA(const std::string& filename);

void ReadTextFileA(const char* filename, std::vector<char>& output);
#ifdef UNICODE
#define ReadTextFile ReadTextFileA
#define ReadBinaryFile ReadBinaryFileA
#else
#define ReadTextFile ReadTextFileA
#define ReadBinaryFile ReadBinaryFileA
#endif

#endif

bool   SAFE_DELETE(HashList* objList);
bool   SAFE_DELETE(Components* objList);
bool   SAFE_DELETE(GameObjectPool* objList);
bool   SAFE_DELETE(ComponentList* objList);
bool   SAFE_DELETE(GameObjectList* objList);


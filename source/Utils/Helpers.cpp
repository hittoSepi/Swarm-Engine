#pragma once
#include "pch.h"
#include "Utils/Helpers.h"


#ifdef _WIN32
const char* ReadTextFile(const char* filename)
{
	std::ifstream f(filename);
	std::string line, result;
	while (getline (f, line)) {
		result += line +"\n";
	}
	return result.c_str();
}


void ReadTextFile(const char* filename, std::vector<char> &output)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {

		throw std::runtime_error("failed to open file: " + std::string(filename));
    }
}

#endif


size_t Hash(const void *ptr, size_t size, size_t result)
{
    for (size_t i = 0; i < size; ++i)
    {
        result = (result * 16777619) ^ ((char *)ptr)[i];
    }

    return result;
}

size_t HashString(const char *str, size_t result)
{
    return Hash(str, strlen(str), result);
}

size_t HashString(const std::string &str, size_t result)
{
    return HashString(str.c_str(), result);
}

size_t HashInt(const int type, size_t result) { return Hash(&type, sizeof(int), result); }
size_t HashFloat(const float type, size_t result) { return Hash(&type, sizeof(float), result); }
size_t HashPtr(const void *type, size_t result) { return Hash(&type, sizeof(void *), result); }

bool SAFE_DELETE(Components* objList)
{
	ComponentIt iter = objList->begin();
	while(iter !=  objList->end())
	{
		delete &iter;
	    iter++;
	}
	objList->clear();
	delete objList;
	return true;
}

bool SAFE_DELETE(HashList* objList)
{
	HashListIt iter = objList->begin();
	while(iter != objList->end())
	{
	     delete &iter;
	     iter++;
	}
	objList->clear();
	delete objList;
	return true;
}

bool SAFE_DELETE(GameObjectList* objList)
{
	GameObjectListIt iter = objList->begin();
	while(iter != objList->end())
	{
	     delete &iter;
	     iter++;
	}
	objList->clear();
	delete objList;
	return true;
}


bool SAFE_DELETE(GameObjects* objList)
{
	GameObjectPoolIt iter = objList->begin();
	while(iter != objList->end())
	{
	     delete &iter;
	     iter++;
	}
	objList->clear();
	delete objList;
	return true;
}



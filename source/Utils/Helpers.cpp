#pragma once
#include "pch.h"
#include "Utils/Helpers.h"
#include <regex>


// filter = "*.frag;*.vert
std::vector<std::string> GetFileListing(std::string path, std::string filename, std::string extensions) {

	std::string extDelim = "|";
	std::vector<std::string> files;
	LogInfo("Searching for: " << filename << "." << extensions);
	
	std::string f = "";
	std::string ext = "";
	
	if (filename == "*")
		f = "(.*)";
	else
		f = "(" + filename + ")";

	if (extensions == "*")
		ext = "(.*)";
	else
		ext = "(" + extensions + ")";

	auto reg = std::regex("(" + path + ")" + f + "." + ext);
	LogVerbose("Regex: (" << path << ")" << f << "." << ext);
	
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		LogVerbose(entry.path());
		if (std::regex_match(entry.path().generic_string(), reg)) {
			LogInfo("Found: " << entry.path());
			files.push_back(entry.path().generic_string());
		}
	}

	return files;
}



std::vector<char> ReadBinaryFileA(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}


#ifdef _WIN32
const char* ReadTextFileA(const char* filename)
{
	std::ifstream f(filename);
	std::string line, result;
	while (getline(f, line)) {
		result += line + "\n";
	}
	return result.c_str();
}


std::string ReadTextFileA(std::string filename)
{
	std::ifstream f(filename);
	std::string line, result;
	while (getline(f, line)) {
		result += line + "\n";
	}
	return result.c_str();
}


void ReadTextFileA(const char* filename, std::vector<char>& output)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {

		throw std::runtime_error("failed to open file: " + std::string(filename));
	}
}

#endif


size_t Hash(const void* ptr, size_t size, size_t result)
{
	for (size_t i = 0; i < size; ++i)
	{
		result = (result * 16777619) ^ ((char*)ptr)[i];
	}

	return result;
}

size_t HashString(const char* str, size_t result)
{
	return Hash(str, strlen(str), result);
}

size_t HashString(const std::string& str, size_t result)
{
	return HashString(str.c_str(), result);
}

size_t HashInt(const int type, size_t result) { return Hash(&type, sizeof(int), result); }
size_t HashFloat(const float type, size_t result) { return Hash(&type, sizeof(float), result); }
size_t HashPtr(const void* type, size_t result) { return Hash(&type, sizeof(void*), result); }

bool SAFE_DELETE(Components* objList)
{
	ComponentIt iter = objList->begin();
	while (iter != objList->end())
	{
		delete& iter;
		iter++;
	}
	objList->clear();
	delete objList;
	return true;
}

bool SAFE_DELETE(HashList* objList)
{
	HashListIt iter = objList->begin();
	while (iter != objList->end())
	{
		delete& iter;
		iter++;
	}
	objList->clear();
	delete objList;
	return true;
}

bool SAFE_DELETE(GameObjectList* objList)
{
	GameObjectListIt iter = objList->begin();
	while (iter != objList->end())
	{
		delete& iter;
		iter++;
	}
	objList->clear();
	delete objList;
	return true;
}


bool SAFE_DELETE(GameObjects* objList)
{
	GameObjectPoolIt iter = objList->begin();
	while (iter != objList->end())
	{
		delete& iter;
		iter++;
	}
	objList->clear();
	delete objList;
	return true;
}



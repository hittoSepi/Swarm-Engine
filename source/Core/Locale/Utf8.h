#pragma once
#include <string>

inline   const wchar_t* StringToWide(std::string input)
{
	return std::wstring(input.begin(), input.end()).c_str();
}

#pragma once
#include <codecvt>
#include <string>

inline   const wchar_t* StringToWide(std::string input)
{
	return std::wstring(input.begin(), input.end()).c_str();
}

inline std::string WideToString(const wchar_t* wide) {

	LPSTR res = NULL;
	WideCharToMultiByte(CP_ACP,
                               0,
                               wide,
                               -1,
                               res,
                               0, NULL, NULL);
	
	
	return std::string(res);
}

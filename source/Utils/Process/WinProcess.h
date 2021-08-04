#pragma once

std::string execA(std::string cmd);
std::wstring execW(std::wstring cmd);

bool LaunchProcessA(const LPCSTR process,	LPCSTR params);
bool LaunchProcessW(const LPCWSTR process,	LPCWSTR params);

#ifdef UNICODE
#define LaunchProcess LaunchProcessW
#define exec execW
#else
#define LaunchProcess LaunchProcessA
#define exec execA
#endif


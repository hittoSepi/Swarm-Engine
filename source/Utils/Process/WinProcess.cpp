#include "pch.h"
#include "WinProcess.h"
#include <shellapi.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif


std::string execA(std::string cmd) {

	LogInfo(cmd);
	
	std::array<char, 128> buffer;
    std::string result;
	
    auto pipe = popen(cmd.c_str(), "r");

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe)) {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }

    auto rc = pclose(pipe);

    if (rc == EXIT_SUCCESS) { // == 0

    } else if (rc == EXIT_FAILURE) {  // EXIT_FAILURE is not used by all programs, maybe needs some adaptation.
		LogWarning("EXIT_FAILURE");
    }
    return result;
}

std::wstring execW(std::wstring cmd) {

	LogInfo(WideToString(cmd.c_str()));
	std::array<wchar_t, 128> buffer;
    std::wstring result;
	
    auto pipe = popen(WideToString(cmd.c_str()).c_str(), "r"); // get rid of shared_ptr

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe)) {
        if (fgetws(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }

    auto rc = pclose(pipe);

    if (rc == EXIT_SUCCESS) { // == 0

    } else if (rc == EXIT_FAILURE) {  // EXIT_FAILURE is not used by all programs, maybe needs some adaptation.

    }
    return result;
}



/// <summary>
/// Launch Process ANSI
/// </summary>
bool LaunchProcessA(const LPCSTR process, LPCSTR params)
{
	LPCSTR _process;
	LPCSTR _params = NULL;

	if (strlen(process) > 0)
		_process = process;
	else
		return false;

	if (strlen(params) > 0)
		_params = params;

	ShellExecuteA(nullptr, "open", _process, _params, nullptr, SW_SHOWDEFAULT);
	return true;
}

/// <summary>
/// Launch Process UNICODE
/// </summary>
bool LaunchProcessW(const LPCWSTR process, LPCWSTR params)
{
	LPCWSTR _process;
	LPCWSTR _params = NULL;

	if (wcslen(process) > 0)
		_process = process;
	else return false;

	if (wcslen(params) > 0)
		_params = params;

	ShellExecuteW(nullptr, L"open", _process, _params, nullptr, SW_SHOWDEFAULT);
	return true;
}

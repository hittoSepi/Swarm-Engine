// SwarmApp.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Game.h"


/*
void Test()
{
	for(int i = 0; i < 10000; i++)
	{
		LogInfo(std::to_string(i));
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void Test2()
{
	for(int i = 0; i < 10000; i++)
	{
		LogInfo(std::to_string(i));
		std::this_thread::sleep_for(std::chrono::milliseconds(333));
	}
}

ThreadPool *pool = new ThreadPool(32);
std::function<void()> JobFunc = Test;
std::function<void()> JobFunc2 = Test2;
*/

int APIENTRY WinMain (
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
	CreateConsole();
	std::cout << "******* SWARM ENGINE v0.1 *******\n\n";
	
	
	Application::run(new GameRenderer(), SwarmConfig());

	//delete pool;
	return 0;
}
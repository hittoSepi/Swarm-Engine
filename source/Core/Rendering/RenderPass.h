#pragma once

#include <string>

class SubPass;

class RenderPass
{
public:
	RenderPass(std::string debugName):
		debugName(debugName)
	{}

	virtual void quit() = 0;
		
protected:
	std::string debugName;
};
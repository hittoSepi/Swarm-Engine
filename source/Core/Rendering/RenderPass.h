#pragma once

#include <string>

class SubPass;

class RenderPass
{
public:
	RenderPass(std::string debugName):
		debugName(debugName)
	{}
		
protected:
	std::string debugName;
};
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

private:
};

class SubPass: public RenderPass
{
public:
	SubPass(std::string debugName):
		RenderPass(debugName)
	{
		
	}
};
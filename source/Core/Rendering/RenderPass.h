#pragma once

class SubPass;

class RenderPass
{
public:
	RenderPass(std::string debugName):
		debugName(debugName)
	{}
	
	virtual void addSubPass(SubPass* pass) = 0;
protected:

private:
	std::string debugName;
};

class SubPass: public RenderPass
{
public:
	SubPass(std::string debugName):
		RenderPass(debugName)
	{
		
	}
};
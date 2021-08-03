#pragma once

class Device
{
public:
	class Options
	{
	public:
		
		bool vsync;
		
	};

	virtual ~Device() = 0;
	
	virtual void init() = 0;
	virtual void quit() = 0;

protected:
	Device(Options &opts);

	

private:

	
};
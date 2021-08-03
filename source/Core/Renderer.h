// <file> Renderer.h </file>
// SwarmEngineDLL
// Copyright © Seppo Laine
// 01.08.2021
#pragma once
//#include "Core/Api/Device.h"

class Clock;
class Framerate;
struct KeyboardEvent;
struct MouseEvent;

struct SwarmConfig
{
	WindowBase::Options windowOptions;		   ///< Controls window creation
	Device::Options deviceOptions;		   ///< Controls device creation
	bool suppressInput = false;		   ///< Suppress all keyboard and mouse input (other than escape to terminate)
	bool showMessageBoxOnError = true; ///< Show message box on framework/API errors.
	float timeScale = 1.0f;			   ///< A scaling factor for the time elapsed between frames
	bool pauseTime = false;			   ///< Control whether or not to start the clock when the sample start running
	bool showUI = true;				   ///< Show the UI
};

class Framework
{
public:
	virtual void				init() = 0;
	virtual void				renderFrame() = 0;
	virtual void				quit() = 0;
	virtual const SwarmConfig	&getConfig() = 0;
	virtual Framerate			*getFPS() = 0;
	virtual WindowBase			*getWindow() = 0;

};

class Renderer
{
public:
	Renderer() = default;
	virtual ~Renderer() {}

	virtual void onLoad() {}
	virtual void onFrameRender() {}
	virtual void onExit() {}
	virtual void onResizeSwapChain(uint32_t width, uint32_t height) {}

	virtual bool onKeyEvent(const KeyboardEvent &keyEvent) { return false; }
	virtual bool onMouseEvent(const MouseEvent &mouseEvent) { return false; }

	Renderer(const Renderer &) = delete;
	Renderer &operator=(const Renderer &) = delete;
};
#pragma once

#include "Core/Input/InputEvents.h"

class JobSystem;
class Framerate;

class Application: public Framework, public Window::ICallbacks
{
public:
	~Application();
	
	// Framework virtuals
	void				init() override;
	void				renderFrame() override;
	void				quit() override;
	
	Framerate			*getFPS() override;
	WindowBase			*getWindow() override;
	const SwarmConfig	&getConfig() override;

	// Callback virtuals
	void handleWindowSizeChange() override;
	void handleRenderFrame() override;
	void handleKeyboardEvent(const KeyboardEvent &keyEvent) override;
	void handleMouseEvent(const MouseEvent &mouseEvent) override;
	void handleDroppedFile(const std::string &filename) override;
	
	static void run(Renderer *_renderer, const SwarmConfig &conf);
	
private:
	Application(Renderer *_renderer);

	void runInternal(const SwarmConfig &conf);
	
	std::set<KeyboardEvent::Key> pressedKeys;
	
	SwarmConfig		config;
	Renderer		*renderer	= nullptr;
	Device			*device		= nullptr; // gpu
	Window			*window		= nullptr;
	Framerate		*fps		= nullptr;
	JobSystem::Ptr	jobSystem;
};

#pragma once

#include "Core/Input/InputEvents.h"
#include "Core/Renderer.h"
#include "Vulkan/VulkanApi.h"

class JobSystem;
class Framerate;

class Application: public Framework, public Window::ICallbacks
{
public:
	static void run(Renderer *_renderer, const EngineConfig &conf);
protected:
	
	// Framework virtuals
	void				init() override;
	void				renderFrame() override;
	void				quit() override;
	
	Framerate			*getFPS() override;
	WindowBase			*getWindow() override;
	const EngineConfig	&getConfig() override;

	// Callback virtuals
	void handleWindowSizeChange() override;
	void handleRenderFrame() override;
	void handleKeyboardEvent(const KeyboardEvent &keyEvent) override;
	void handleMouseEvent(const MouseEvent &mouseEvent) override;
	void handleDroppedFile(const std::string &filename) override;
	
	
private:
	Application(Renderer *_renderer);
	~Application() = default;

	void runInternal(const EngineConfig &conf);
	
	std::set<KeyboardEvent::Key> pressedKeys;
	
	EngineConfig	config;					// engine config

	
//	SWDevice		graphicsApi;					
	Window			*window			= nullptr;  // main window
	RenderingApi	*graphicsApi	= nullptr;	// gpu
	Renderer		*renderer		= nullptr;	// render handler
	Framerate		*fps			= nullptr;
	JobSystem::Ptr	jobSystem;
};

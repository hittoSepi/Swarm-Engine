#include "pch.h"
#include "Core/Application.h"

Application::Application(Renderer* _renderer) :
	renderer(_renderer)
{
	if (renderer == nullptr)
	{
		LogError(std::string("No renderer"));
	}
}


void Application::run(Renderer* renderer, const EngineConfig &conf)
{
	Application app(renderer);
	try
	{
		app.runInternal(conf);
	}
	catch (const std::exception& e)
	{
		LogError(std::string("Caught exception:\n\n" + std::string(e.what())));
	}
}


void Application::runInternal(const EngineConfig &conf)
{
	config = conf;
	
	init();

	window->messageLoop();
	
	
	jobSystem->finish();
	delete fps;
	
	if(renderer != nullptr){
		renderer->onExit();
		delete renderer;
	}

	if(graphicsApi != nullptr) {
		graphicsApi->quit();
		delete graphicsApi;
	}

	delete window;
}


void Application::init()
{
	// start subsystems
	Clock::start();
	
	fps = new Framerate();
	fps->Update();

	jobSystem = JobSystem::create(config.numThreads);	// Start Threads
	
	// create main dinwo
	window = new Window(config.windowOptions, this);
	window->init();

	InitShaderCache("Data/ShaderData/", "Data/ShaderCache/");
	
	// init Graphics Api here
	if(config.graphicsApi == GraphicsAPI::VULKAN) {
		graphicsApi = (VulkanApi*)VulkanApi::create(window, config.deviceOptions);
	}

	if(graphicsApi != nullptr)
		graphicsApi->init();
	else
		throw std::runtime_error("Graphics Api is NULL");
	
	// renderer->OnLoad(renderecontext) set render context
}


void Application::renderFrame()
{
	Clock::update();
	fps->Update();
	renderer->onFrameRender();
}


void Application::quit()
{
	window->quit();
}


void Application::handleRenderFrame()
{
	renderFrame();
}


void Application::handleKeyboardEvent(const KeyboardEvent& keyEvent)
{
	if (keyEvent.type == KeyboardEvent::Type::KeyPressed)       pressedKeys.insert(keyEvent.key);
	else if (keyEvent.type == KeyboardEvent::Type::KeyReleased) pressedKeys.erase(keyEvent.key);

	if (renderer && renderer->onKeyEvent(keyEvent)) return;

	if (keyEvent.key == KeyboardEvent::Key::Escape)
	{
		quit();
		return;
	}
}


void Application::handleMouseEvent(const MouseEvent& mouseEvent)
{
	if (renderer && renderer->onMouseEvent(mouseEvent)) return;
}


void Application::handleWindowSizeChange()
{
	// TODO!
	  if (!graphicsApi) return;
    // Tell the graphicsApi to resize the swap chain
    //auto winSize = window->getClientAreaSize();
    //auto pBackBufferFBO = graphicsApi->resizeSwapChain(winSize.x, winSize.y);
    //auto width = pBackBufferFBO->getWidth();
    //auto height = pBackBufferFBO->getHeight();

    // Recreate target fbo
    //auto pCurrentFbo = mpTargetFBO;
    //mpTargetFBO = Fbo::create2D(width, height, pBackBufferFBO->getDesc());
    //gpDevice->getRenderContext()->blit(pCurrentFbo->getColorTexture(0)->getSRV(), mpTargetFBO->getRenderTargetView(0));

    // Tell the GUI the swap-chain size changed
    //if(mpGui) mpGui->onWindowResize(width, height);

    // Resize the pixel zoom
    //if(mpPixelZoom) mpPixelZoom->onResizeSwapChain(gpDevice->getSwapChainFbo().get());

    // Call the user callback
   // if(renderer) renderer->onResizeSwapChain(width, height);
}


void Application::handleDroppedFile(const std::string& filename)
{
	LogInfo(filename)
}


const EngineConfig& Application::getConfig() { return config; }
WindowBase* Application::getWindow() { return window; }
Framerate* Application::getFPS() { return fps; }






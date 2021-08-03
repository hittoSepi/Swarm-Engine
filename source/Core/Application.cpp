#include "pch.h"


Application::Application(Renderer* _renderer) :
	renderer(_renderer)
{
	if (renderer == nullptr)
	{
		LogError("No renderer");
	}


}


Application::~Application()
{

}


void Application::run(Renderer* renderer, const SwarmConfig& conf)
{
	Application app(renderer);
	try
	{
		app.runInternal(conf);
	}
	catch (const std::exception& e)
	{
		LogError("Caught exception:\n\n" + std::string(e.what()) + "\n\nEnable breaking on exceptions in the debugger to get a full stack trace.");
	}
}


void Application::runInternal(const SwarmConfig &conf)
{
	LogInfo("inir");
	config = conf;
	init();

	LogInfo("main loop")
	window->messageLoop();

	LogInfo("exitting")

	
	if(device != nullptr)
		device->quit();
		delete device;

	if(renderer != nullptr){
		renderer->onExit();
		delete renderer;
	}
	delete fps;
	
}



void Application::init()
{
	// start subsystems
	Clock::start();
	
	fps = new Framerate();
	fps->Update();

	jobSystem = JobSystem::create(16);	// Start Threads
	
	// create main dinwo
	window = new Window(config.windowOptions, this);
	window->init();

	// init device here
	// device->init();
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
	jobSystem->finish();
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
	  if (!device) return;
    // Tell the device to resize the swap chain
    //auto winSize = window->getClientAreaSize();
    //auto pBackBufferFBO = device->resizeSwapChain(winSize.x, winSize.y);
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


const SwarmConfig& Application::getConfig() { return config; }
WindowBase* Application::getWindow() { return window; }
Framerate* Application::getFPS() { return fps; }






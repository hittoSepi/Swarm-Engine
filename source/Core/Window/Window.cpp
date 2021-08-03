#include "pch.h"
#include "Core/Window/Window.h"
#define GLFW_INCLUDE_NONE

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#else // LINUX

// Replace the defines we undef'd in FalcorVK.h, because glfw will need them when it includes Xlib
#define None 0L
#define Bool int
#define Status int
#define Always 2

#define GLFW_EXPOSE_NATIVE_X11
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#endif

#include "Core/ApiCallbacks.h"


Window::Window(Options& opts, ICallbacks* callbacks) :
	WindowBase(opts, callbacks)
{}


Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}


void Window::init()
{

	LogInfo("begin")
	// Set error callback
	glfwSetErrorCallback(ApiCallbacks::errorCallback);

	// Init GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		LogError("GLFW initialization failed");
	}

	//SharedPtr pWindow = SharedPtr(new Window(pCallbacks, options));

	// Create the window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
	uint32_t w = options.width;
	uint32_t h = options.height;

	if (options.mode == WindowMode::Fullscreen)
	{
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		auto mon = glfwGetPrimaryMonitor();
		auto mod = glfwGetVideoMode(mon);
		w = mod->width;
		h = mod->height;
	}
	else if (options.mode == WindowMode::Minimized)
	{
		// Start with window being invisible
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	}

	if (options.resizableWindow == false)
	{
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	window = glfwCreateWindow(w, h, options.title.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
		LogError("Window creation failed!");
	}

	// Init handles
	setMainWindowHandle(glfwGetWin32Window(window));

	updateWindowSize();

	glfwSetWindowUserPointer(window, this);

	// Set callbacks
	glfwSetWindowSizeCallback(window,	ApiCallbacks::windowSizeCallback);
	glfwSetKeyCallback(window,			ApiCallbacks::keyboardCallback);
	glfwSetMouseButtonCallback(window,	ApiCallbacks::mouseButtonCallback);
	glfwSetCursorPosCallback(window,	ApiCallbacks::mouseMoveCallback);
	glfwSetScrollCallback(window,		ApiCallbacks::mouseWheelCallback);
	glfwSetCharCallback(window,			ApiCallbacks::charInputCallback);
	glfwSetDropCallback(window,			ApiCallbacks::droppedFileCallback);

	if (options.mode == WindowMode::Minimized)
	{
		// Iconify and show window to make it available if user clicks on it
		glfwIconifyWindow(window);
		glfwShowWindow(window);
	}
	LogInfo("end")

}


void Window::quit()
{
	glfwSetWindowShouldClose(window, 1);
}


void Window::messageLoop()
{
	callbacks->handleWindowSizeChange();

	if (options.mode != WindowMode::Minimized)
	{
		glfwShowWindow(window);
		glfwFocusWindow(window);
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		callbacks->handleRenderFrame();
	}
}


void Window::pollEvents()
{
	glfwPollEvents();
}


void Window::resize(uint32_t width, uint32_t height)
{
	glfwSetWindowSize(window, width, height);

	// In minimized mode GLFW reports incorrect window size
	if (options.mode == WindowMode::Minimized)
	{
		setWindowSize(width, height);
	}
	else
	{
		updateWindowSize();
	}

	callbacks->handleWindowSizeChange();
}



void Window::setPosition(uint32_t x, uint32_t y)
{
	glfwSetWindowPos(window, x, y);
}


void Window::setWindowTitle(std::string title)
{
	glfwSetWindowTitle(window, title.c_str());
}


void Window::setWindowSize(uint32_t width, uint32_t height)
{
	assert(width > 0 && height > 0);

	options.width = width;
	options.height = height;
	mouseScale.x = 1.0f / (float)options.width;
	mouseScale.y = 1.0f / (float)options.height;
}


void Window::updateWindowSize()
{
	int32_t width, height;
	glfwGetWindowSize(window, &width, &height);
	setWindowSize(width, height);
}






#include "pch.h"
#include "Core/Window/Window.h"
#define GLFW_INCLUDE_NONE

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#else // LINUX

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
	LogInfo("")
	glfwDestroyWindow(window);
	glfwTerminate();
}


void Window::init()
{
	LogInfo("");
	glfwSetErrorCallback(ApiCallbacks::errorCallback);

	if (glfwInit() == GLFW_FALSE)
	{
		throw std::runtime_error("GLFW initialization failed");
	}


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
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	}

	if (options.resizableWindow == false)
	{
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	window = glfwCreateWindow(w, h, options.title.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
		throw std::runtime_error("Window creation failed!");
	}

	setMainWindowHandle(glfwGetWin32Window(window));
	updateWindowSize();

	glfwSetWindowUserPointer(window, this);

	glfwSetWindowSizeCallback(window,	ApiCallbacks::windowSizeCallback);
	glfwSetKeyCallback(window,			ApiCallbacks::keyboardCallback);
	glfwSetMouseButtonCallback(window,	ApiCallbacks::mouseButtonCallback);
	glfwSetCursorPosCallback(window,	ApiCallbacks::mouseMoveCallback);
	glfwSetScrollCallback(window,		ApiCallbacks::mouseWheelCallback);
	glfwSetCharCallback(window,			ApiCallbacks::charInputCallback);
	glfwSetDropCallback(window,			ApiCallbacks::droppedFileCallback);

	if (options.mode == WindowMode::Minimized)
	{
		glfwIconifyWindow(window);
		glfwShowWindow(window);
	}

	LogDebug("end");
}


void Window::quit()
{
	LogInfo("triggered.")
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


void* Window::getNativeWindow() const
{
	return window;
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




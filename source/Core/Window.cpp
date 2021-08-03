#include "pch.h"
#include "Core/Window.h"
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

#include "ApiCallbacks.h"

/*class ApiCallbacks
{
public:
	static void windowSizeCallback(GLFWwindow* pGlfwWindow, int width, int height)
	{
		// We also get here in case the window was minimized, so we need to ignore it
		if (width == 0 || height == 0)
		{
			return;
		}

		Window* pWindow = (Window*)glfwGetWindowUserPointer(pGlfwWindow);
		if (pWindow != nullptr)
		{
			pWindow->resize(width, height); // Window callback is handled in here
		}
	}

	static void keyboardCallback(GLFWwindow* pGlfwWindow, int key, int scanCode, int action, int modifiers)
	{
		KeyboardEvent event;
		if (prepareKeyboardEvent(key, action, modifiers, event))
		{
			Window* pWindow = (Window*)glfwGetWindowUserPointer(pGlfwWindow);
			if (pWindow != nullptr)
			{
				pWindow->callbacks->handleKeyboardEvent(event);
			}
		}
	}

	static void charInputCallback(GLFWwindow* pGlfwWindow, uint32_t input)
	{
		KeyboardEvent event;
		event.type = KeyboardEvent::Type::Input;
		event.codepoint = input;

		Window* pWindow = (Window*)glfwGetWindowUserPointer(pGlfwWindow);
		if (pWindow != nullptr)
		{
			pWindow->callbacks->handleKeyboardEvent(event);
		}
	}

	static void mouseMoveCallback(GLFWwindow* pGlfwWindow, double mouseX, double mouseY)
	{
		Window* pWindow = (Window*)glfwGetWindowUserPointer(pGlfwWindow);
		if (pWindow != nullptr)
		{
			// Prepare the mouse data
			MouseEvent event;
			event.type = MouseEvent::Type::Move;
			event.pos = calcMousePos(mouseX, mouseY, pWindow->getMouseScale());
			event.screenPos = { mouseX, mouseY };
			event.wheelDelta = glm::vec2(0, 0);

			pWindow->callbacks->handleMouseEvent(event);
		}
	}

	static void mouseButtonCallback(GLFWwindow* pGlfwWindow, int button, int action, int modifiers)
	{
		MouseEvent event;
		// Prepare the mouse data
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			event.type = (action == GLFW_PRESS) ? MouseEvent::Type::LeftButtonDown : MouseEvent::Type::LeftButtonUp;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			event.type = (action == GLFW_PRESS) ? MouseEvent::Type::MiddleButtonDown : MouseEvent::Type::MiddleButtonUp;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			event.type = (action == GLFW_PRESS) ? MouseEvent::Type::RightButtonDown : MouseEvent::Type::RightButtonUp;
			break;
		default:
			// Other keys are not supported
			break;
		}

		Window* pWindow = (Window*)glfwGetWindowUserPointer(pGlfwWindow);
		if (pWindow != nullptr)
		{
			// Modifiers
			event.mods = getInputModifiers(modifiers);
			double x, y;
			glfwGetCursorPos(pGlfwWindow, &x, &y);
			event.pos = calcMousePos(x, y, pWindow->getMouseScale());

			pWindow->callbacks->handleMouseEvent(event);
		}
	}

	static void mouseWheelCallback(GLFWwindow* pGlfwWindow, double scrollX, double scrollY)
	{
		Window* pWindow = (Window*)glfwGetWindowUserPointer(pGlfwWindow);
		if (pWindow != nullptr)
		{
			MouseEvent event;
			event.type = MouseEvent::Type::Wheel;
			double x, y;
			glfwGetCursorPos(pGlfwWindow, &x, &y);
			event.pos = calcMousePos(x, y, pWindow->getMouseScale());
			event.wheelDelta = (glm::vec2(float(scrollX), float(scrollY)));

			pWindow->callbacks->handleMouseEvent(event);
		}
	}

	static void errorCallback(int errorCode, const char* poptsription)
	{
		std::string errorMsg = std::to_string(errorCode) + " - " + std::string(poptsription) + "\n";
		LogError(errorMsg.c_str());
	}

	static void droppedFileCallback(GLFWwindow* pGlfwWindow, int count, const char** paths)
	{
		Window* pWindow = (Window*)glfwGetWindowUserPointer(pGlfwWindow);
		if (pWindow)
		{
			for (int i = 0; i < count; i++)
			{
				std::string filename(paths[i]);
				pWindow->callbacks->handleDroppedFile(filename);
			}
		}
	}
private:

	static inline KeyboardEvent::Key glfwToFalcorKey(int glfwKey)
	{
		static_assert(GLFW_KEY_ESCAPE == 256, "GLFW_KEY_ESCAPE is expected to be 256");
		if (glfwKey < GLFW_KEY_ESCAPE)
		{
			// Printable keys are expected to have the same value
			return (KeyboardEvent::Key)glfwKey;
		}

		switch (glfwKey)
		{
		case GLFW_KEY_ESCAPE:
			return KeyboardEvent::Key::Escape;
		case GLFW_KEY_ENTER:
			return KeyboardEvent::Key::Enter;
		case GLFW_KEY_TAB:
			return KeyboardEvent::Key::Tab;
		case GLFW_KEY_BACKSPACE:
			return KeyboardEvent::Key::Backspace;
		case GLFW_KEY_INSERT:
			return KeyboardEvent::Key::Insert;
		case GLFW_KEY_DELETE:
			return KeyboardEvent::Key::Del;
		case GLFW_KEY_RIGHT:
			return KeyboardEvent::Key::Right;
		case GLFW_KEY_LEFT:
			return KeyboardEvent::Key::Left;
		case GLFW_KEY_DOWN:
			return KeyboardEvent::Key::Down;
		case GLFW_KEY_UP:
			return KeyboardEvent::Key::Up;
		case GLFW_KEY_PAGE_UP:
			return KeyboardEvent::Key::PageUp;
		case GLFW_KEY_PAGE_DOWN:
			return KeyboardEvent::Key::PageDown;
		case GLFW_KEY_HOME:
			return KeyboardEvent::Key::Home;
		case GLFW_KEY_END:
			return KeyboardEvent::Key::End;
		case GLFW_KEY_CAPS_LOCK:
			return KeyboardEvent::Key::CapsLock;
		case GLFW_KEY_SCROLL_LOCK:
			return KeyboardEvent::Key::ScrollLock;
		case GLFW_KEY_NUM_LOCK:
			return KeyboardEvent::Key::NumLock;
		case GLFW_KEY_PRINT_SCREEN:
			return KeyboardEvent::Key::PrintScreen;
		case GLFW_KEY_PAUSE:
			return KeyboardEvent::Key::Pause;
		case GLFW_KEY_F1:
			return KeyboardEvent::Key::F1;
		case GLFW_KEY_F2:
			return KeyboardEvent::Key::F2;
		case GLFW_KEY_F3:
			return KeyboardEvent::Key::F3;
		case GLFW_KEY_F4:
			return KeyboardEvent::Key::F4;
		case GLFW_KEY_F5:
			return KeyboardEvent::Key::F5;
		case GLFW_KEY_F6:
			return KeyboardEvent::Key::F6;
		case GLFW_KEY_F7:
			return KeyboardEvent::Key::F7;
		case GLFW_KEY_F8:
			return KeyboardEvent::Key::F8;
		case GLFW_KEY_F9:
			return KeyboardEvent::Key::F9;
		case GLFW_KEY_F10:
			return KeyboardEvent::Key::F10;
		case GLFW_KEY_F11:
			return KeyboardEvent::Key::F11;
		case GLFW_KEY_F12:
			return KeyboardEvent::Key::F12;
		case GLFW_KEY_KP_0:
			return KeyboardEvent::Key::Keypad0;
		case GLFW_KEY_KP_1:
			return KeyboardEvent::Key::Keypad1;
		case GLFW_KEY_KP_2:
			return KeyboardEvent::Key::Keypad2;
		case GLFW_KEY_KP_3:
			return KeyboardEvent::Key::Keypad3;
		case GLFW_KEY_KP_4:
			return KeyboardEvent::Key::Keypad4;
		case GLFW_KEY_KP_5:
			return KeyboardEvent::Key::Keypad5;
		case GLFW_KEY_KP_6:
			return KeyboardEvent::Key::Keypad6;
		case GLFW_KEY_KP_7:
			return KeyboardEvent::Key::Keypad7;
		case GLFW_KEY_KP_8:
			return KeyboardEvent::Key::Keypad8;
		case GLFW_KEY_KP_9:
			return KeyboardEvent::Key::Keypad9;
		case GLFW_KEY_KP_DECIMAL:
			return KeyboardEvent::Key::KeypadDel;
		case GLFW_KEY_KP_DIVIDE:
			return KeyboardEvent::Key::KeypadDivide;
		case GLFW_KEY_KP_MULTIPLY:
			return KeyboardEvent::Key::KeypadMultiply;
		case GLFW_KEY_KP_SUBTRACT:
			return KeyboardEvent::Key::KeypadSubtract;
		case GLFW_KEY_KP_ADD:
			return KeyboardEvent::Key::KeypadAdd;
		case GLFW_KEY_KP_ENTER:
			return KeyboardEvent::Key::KeypadEnter;
		case GLFW_KEY_KP_EQUAL:
			return KeyboardEvent::Key::KeypadEqual;
		case GLFW_KEY_LEFT_SHIFT:
			return KeyboardEvent::Key::LeftShift;
		case GLFW_KEY_LEFT_CONTROL:
			return KeyboardEvent::Key::LeftControl;
		case GLFW_KEY_LEFT_ALT:
			return KeyboardEvent::Key::LeftAlt;
		case GLFW_KEY_LEFT_SUPER:
			return KeyboardEvent::Key::LeftSuper;
		case GLFW_KEY_RIGHT_SHIFT:
			return KeyboardEvent::Key::RightShift;
		case GLFW_KEY_RIGHT_CONTROL:
			return KeyboardEvent::Key::RightControl;
		case GLFW_KEY_RIGHT_ALT:
			return KeyboardEvent::Key::RightAlt;
		case GLFW_KEY_RIGHT_SUPER:
			return KeyboardEvent::Key::RightSuper;
		case GLFW_KEY_MENU:
			return KeyboardEvent::Key::Menu;
		default:
			should_not_get_here();
			return (KeyboardEvent::Key)0;
		}
	}

	static inline InputModifiers getInputModifiers(int mask)
	{
		InputModifiers mods;
		mods.isAltDown = (mask & GLFW_MOD_ALT) != 0;
		mods.isCtrlDown = (mask & GLFW_MOD_CONTROL) != 0;
		mods.isShiftDown = (mask & GLFW_MOD_SHIFT) != 0;
		return mods;
	}

	static inline float2 calcMousePos(double xPos, double yPos, const float2& mouseScale)
	{
		float2 pos = float2(float(xPos), float(yPos));
		pos *= mouseScale;
		return pos;
	}

	static inline bool prepareKeyboardEvent(int key, int action, int modifiers, KeyboardEvent& event)
	{
		if (action == GLFW_REPEAT || key == GLFW_KEY_UNKNOWN)
		{
			return false;
		}

		event.type = (action == GLFW_RELEASE ? KeyboardEvent::Type::KeyReleased : KeyboardEvent::Type::KeyPressed);
		event.key = glfwToFalcorKey(key);
		event.mods = getInputModifiers(modifiers);
		return true;
	}
};*/


Window::Window(Options& opts, ICallbacks* callbacks) :
	WindowBase(opts, callbacks)
{

}


Window::~Window()
{
	glfwDestroyWindow(window);
    glfwTerminate();
}


void Window::init()
{
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
        glfwSetWindowSizeCallback(window, ApiCallbacks::windowSizeCallback);
        glfwSetKeyCallback(window, ApiCallbacks::keyboardCallback);
        glfwSetMouseButtonCallback(window, ApiCallbacks::mouseButtonCallback);
        glfwSetCursorPosCallback(window, ApiCallbacks::mouseMoveCallback);
        glfwSetScrollCallback(window, ApiCallbacks::mouseWheelCallback);
        glfwSetCharCallback(window, ApiCallbacks::charInputCallback);
        glfwSetDropCallback(window, ApiCallbacks::droppedFileCallback);

        if (options.mode == WindowMode::Minimized)
        {
            // Iconify and show window to make it available if user clicks on it
            glfwIconifyWindow(window);
            glfwShowWindow(window);
        }
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
	
	while(!glfwWindowShouldClose(window))
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

	options.width  = width;
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





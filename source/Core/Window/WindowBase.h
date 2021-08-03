#pragma once
#include <glm/vec2.hpp>

struct KeyboardEvent;
struct MouseEvent;

enum class WindowMode
{
	Windowed,
	Borderless,
	Fullscreen,
	Minimized
};


/// Window abstraction class
class WindowBase
{
public:
	#ifdef _WIN32
	using WindowHandle = HWND;
	#else
	using WindowHandle = int;
	#endif

	using uint2  = glm::i32vec2;
	using float2 = glm::vec2;
	
	class ICallbacks
	{
	public:
		virtual void handleWindowSizeChange() = 0;
		virtual void handleRenderFrame() = 0;
		virtual void handleKeyboardEvent(const KeyboardEvent &keyEvent) = 0;
		virtual void handleMouseEvent(const MouseEvent &mouseEvent) = 0;
		virtual void handleDroppedFile(const std::string &filename) = 0;
	};
		
	struct Options
	{
		uint32_t width				= 1280;
		uint32_t height				= 720;
		std::string title			= "Swarm Engine";
		bool resizableWindow		= false;
		WindowMode mode				= WindowMode::Windowed;
	};

	WindowBase(Options &opts, ICallbacks *callbacks):
		options(opts), callbacks(callbacks),
		mouseScale(1.0f / (float)opts.width, 1.0f / (float)opts.height)
	{}
		
	virtual ~WindowBase() { };

	virtual void init() = 0; /// Initialize and open window
	virtual void quit() = 0; /// Prepare to quit

	virtual void messageLoop() = 0; /// Main MessageLoop
	virtual void pollEvents() = 0;	/// Force poll events

	virtual void*		getNativeWindow() const = 0;
	virtual void		setWindowTitle(std::string title) = 0;
	virtual void		setPosition(uint32_t x, uint32_t y) = 0;
	virtual void		resize(uint32_t width, uint32_t height) = 0;
	virtual uint2		getClientAreaSize() = 0;

	const std::string	getWindowTitle() { return options.title; }
	const WindowHandle	&getWindowHandle() const { return windowHandle; }
	const Options		&getOptions() const { return options; }
	const float2		getMouseScale() const { return mouseScale; }

protected:
	void setMainWindowHandle(WindowHandle handle) { windowHandle = handle; }
	
	Options			options			= Options();
	WindowHandle	windowHandle	= nullptr;
	ICallbacks		*callbacks		= nullptr;
	float2			mouseScale;

};

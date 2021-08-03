#pragma once



class KeyboardEvent;
class MouseEvent;

/// Window abstraction class
class   WindowBase
{
public:
	friend class ApiCallbacks;

	#ifdef _WIN32
	using WindowHandle = HWND;
	#else
	using WindowHandle = int;
	#endif
	
	class ICallbacks
	{
	public:
		virtual void handleWindowSizeChange() = 0;
		virtual void handleRenderFrame() = 0;
		virtual void handleKeyboardEvent(const KeyboardEvent &keyEvent) = 0;
		virtual void handleMouseEvent(const MouseEvent &mouseEvent) = 0;
		virtual void handleDroppedFile(const std::string &filename) = 0;
	};

	enum class WindowMode
	{
		Windowed,
		Borderless,
		Fullscreen
	};

	struct Options
	{
		uint32_t width			= 1280;
		uint32_t height			= 720;
		std::string title		= "Swarm Engine";
		bool resizableWindow	= false;
		WindowBase::WindowMode mode	= WindowBase::WindowMode::Windowed;
	};

	WindowBase(Options &opts, ICallbacks *callbacks):
		options(opts), callbacks(callbacks)
	{}
		
	virtual ~WindowBase() = 0;

	virtual void init() = 0; /// Initialize and open window
	virtual void quit() = 0; /// Prepare to quit

	virtual void messageLoop() = 0; /// Main MessageLoop
	virtual void pollEvents() = 0;	/// Force poll events

	virtual void setWindowTitle(std::string title) = 0;
	virtual void setPosition(uint32_t x, uint32_t y) = 0;
	virtual void resize(uint32_t width, uint32_t height) = 0;
	virtual void setWindowSize(uint32_t width, uint32_t height) = 0;

	const WindowHandle &getWindowHandle() const { return windowHandle; }
	const Options &getOptions() const { return options; }

protected:
	Options options = Options();
	WindowHandle windowHandle = nullptr;
	ICallbacks *callbacks = nullptr;
};

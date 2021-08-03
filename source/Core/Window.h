#pragma once

struct GLFWwindow;

class Window: public WindowBase
{
public:
	friend class ApiCallbacks;
	
	Window(Options &opts, ICallbacks *callbacks);
	~Window();

	void init() override; /// Initialize and open window
	void quit() override; /// Prepare to quit

	void messageLoop() override; /// Main MessageLoop
	void pollEvents() override;	/// Force poll events

	void setWindowTitle(std::string title) override;
	void setPosition(uint32_t x, uint32_t y) override;
	void resize(uint32_t width, uint32_t height) override;
protected:
	void setWindowSize(uint32_t width, uint32_t height);
	void updateWindowSize();
	

private:
	GLFWwindow *window = nullptr;
};


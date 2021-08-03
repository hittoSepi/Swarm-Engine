#pragma once

struct GLFWwindow;

class Window : public WindowBase
{
public:
	friend class ApiCallbacks;

	using uint2 = glm::i32vec2;
	using float2 = glm::vec2;
	
	Window(Options& opts, ICallbacks* callbacks);
	~Window() override;

	void init() override; /// Initialize and open window
	void quit() override; /// Prepare to quit

	void messageLoop() override; /// Main MessageLoop
	void pollEvents() override;	/// Force poll events

	void* getNativeWindow() const override;
	
	void setWindowTitle(std::string title) override;
	void setPosition(uint32_t x, uint32_t y) override;
	void resize(uint32_t width, uint32_t height) override;
	uint2 getClientAreaSize() override
	{
		return uint2(options.width, options.height);
	}

protected:
	void setWindowSize(uint32_t width, uint32_t height);
	void updateWindowSize();


private:
	GLFWwindow* window = nullptr;
};


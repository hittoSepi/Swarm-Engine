#pragma once
#include "swarm.h"
#include "glm/glm.hpp"

static const glm::vec4 kClearColor(0.38f, 0.52f, 0.10f, 1);


class GameRenderer: public Renderer
{
public:
	GameRenderer();

	void onLoad() override;
	void onExit() override;
	void onFrameRender() override;
	void onResizeSwapChain(uint32_t width, uint32_t height) override;
	
	bool onKeyEvent(const KeyboardEvent& keyEvent) override { return false; }
	bool onMouseEvent(const MouseEvent& mouseEvent) override
	{
		//LogInfo(mouseEvent.typeString);
		return true;
	}
	
	void loadScene();
};

#pragma once
#include "Core/Rendering/Viewport.h"


class VulkanScissor: public Scissor, public VkRect2D
{
public:
	VulkanScissor( const iRect& scissors_area):
		Scissor(scissors_area),
		VkRect2D()
	{
		VkOffset2D    offset_;
		VkExtent2D    extent_;
		offset_.x = scissors_area.x;
		offset_.y = scissors_area.y;

		extent_.width = scissors_area.width;
		extent_.height = scissors_area.height;

		offset = offset_;
		extent = extent_;
	}

};

class VulkanViewport : public Viewport, public VkViewport
{
public:
	VulkanViewport(iRect view_area, float2 depth);
	~VulkanViewport() override = default;
protected:
	void setViewAreaImpl() override;
	void setPositionImpl() override;
	void setSizeImpl() override;
	void setDepthImpl() override;
	
};

#include "pch.h"
#include "Core/Vulkan/Render/VulkanViewport.h"


VulkanViewport::VulkanViewport(iRect view_area, float2 depth) :
	Viewport(view_area, depth),
	VkViewport(
		{ (float)view_area.x,
		(float)view_area.y,
		(float)view_area.width,
		(float)view_area.height,
		depth.x,
		depth.y
		})
{}

void VulkanViewport::setViewAreaImpl()
{
	setPositionImpl();
	setSizeImpl();
}


void VulkanViewport::setPositionImpl()
{
	x = (float)view_area.x;
	y = (float)view_area.y;
}

void VulkanViewport::setSizeImpl()
{
	width = (float)view_area.width;
	height = (float)view_area.height;
}

void VulkanViewport::setDepthImpl()
{
	minDepth = min_depth;
	maxDepth = max_depth;
}


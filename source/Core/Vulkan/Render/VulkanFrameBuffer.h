#pragma once
using FrameBuffer = VkFramebuffer;

class VulkanFrameBuffer
{

public:
	VulkanFrameBuffer(const VulkanDevices& devices, VkRenderPass renderPass, const VkImageView* attachmentViews, int width, int height);
	~VulkanFrameBuffer() = default;

	void quit();

	FrameBuffer getFrameBuffer() { return buffer; }

protected:
	int width, height;

	const VulkanDevices& devices;
	FrameBuffer buffer;
};

#pragma once
#include "VulkanApi.h"
#include <vulkan/vulkan.h>
#include <vector>

class VulkanRenderingPipeline;

class VulkanCommandPool
{
public:
	VulkanCommandPool(const VulkanDevices& devices, VulkanRenderingPipeline* pipeline, size_t framebufferCount);
	~VulkanCommandPool() = default;

	void begin(int index);
	void excecute(VulkanRenderingPipeline* pipeline);
	void end();
	void quit();

	VkCommandBuffer getBuffer(int index) { return buffers[index]; }
 
	VkCommandPool getCommandPool() { return commandPool;}

protected:
	VulkanRenderingPipeline* pipeline;
	VulkanDevices devices;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> buffers;
	
	VkCommandPoolCreateInfo poolInfo{};

};

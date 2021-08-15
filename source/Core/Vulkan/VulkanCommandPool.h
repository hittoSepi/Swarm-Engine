#pragma once
#include "VulkanApi.h"
#include <vulkan/vulkan.h>
#include <vector>

#include "Core/Vulkan/Vertex/VulkanVertexBuffer.h"
#include "Vertex/Vertex.h"


class VulkanRenderingPipeline;

VkCommandBuffer beginSingleTimeCommands(VkDevice device, VulkanCommandPool* commandPool);
void endSingleTimeCommands(VkDevice device, VkQueue graphicsQueue, VulkanCommandPool* commandPool, VkCommandBuffer commandBuffer);

class VulkanCommandPool
{
public:
	VulkanCommandPool(const VulkanDevices& devices, VulkanRenderingPipeline* pipeline, const size_t& framebufferCount);
	~VulkanCommandPool() = default;

	void begin(int index);
	void excecute(VulkanRenderingPipeline* pipeline);
	void end();
	void quit();

	void createBuffers();
	
	VkCommandBuffer getBuffer(int index) { return buffers[index]; }
	VkCommandPool getCommandPool() { return commandPool;}

protected:

	const size_t framebufferCount;
	VulkanRenderingPipeline* pipeline;
	VulkanDevices devices;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> buffers;
	VulkanVertexBuffer *vertexBuffer;
	VulkanIndexbuffer	*indexBuffer;
	VkCommandPoolCreateInfo poolInfo{};
	
};

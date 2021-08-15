#pragma once

#include "Core/Vulkan/Vertex/Vertex.h"
#include <glm/matrix.hpp>

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

//////////////////////////////
// Base VulkanBuffer
class VulkanBuffer
{
public:
	VulkanBuffer(const VulkanDevices& devices, VulkanCommandPool *commandPool);
	virtual ~VulkanBuffer();
	
	virtual void create() = 0;
	VkBuffer getBuffer() { return buffer; }

protected:
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	
	VulkanCommandPool		*commandPool;
	VulkanDevices			devices;
	VkBuffer				buffer;
	VkDeviceMemory			bufferMemory;
};



//////////////////////////////
// VertexBuffer vector3 + rgba 
class VulkanVertexBuffer: public VulkanBuffer
{
public:
	VulkanVertexBuffer(const VulkanDevices& devices, VulkanCommandPool *commandPool);
	VulkanVertexBuffer(const VulkanDevices& devices, VulkanCommandPool *commandPool, const std::vector<Vertex3>& vertices);

	void create() override;
	
	void addVertice(const Vertex3& vertex)
	{
		vertices.push_back(vertex);
	}

	void addVertices(const std::vector<Vertex3>& vertices_)
	{
		LogInfo("");
		vertices = vertices_;
	}

	uint32_t size() { return vertices.size(); }
	

protected:
	std::vector<Vertex3>	vertices;
};


//////////////////////////////
// IndexBuffer
class VulkanIndexbuffer: public VulkanBuffer
{
public:
	VulkanIndexbuffer(const VulkanDevices& devices, VulkanCommandPool *commandPool);
	VulkanIndexbuffer(const VulkanDevices& devices, VulkanCommandPool *commandPool, const std::vector<uint32_t>& indices);

	void create() override;

	void addIndex(uint32_t index)
	{
		indices.push_back(index);
	}

	void addIndices(const std::vector<uint32_t> indices_)
	{
		indices = indices_;
	}

	uint32_t size() { return indices.size(); }

protected:
	std::vector<uint32_t> indices;
};


//////////////////////////////
// UniformBuffer


struct mvpUniform
{
	mat4 model;
    mat4 view;
    mat4 proj;
};

class VulkanUniformBuffer
{
public:
	VulkanUniformBuffer(const VulkanDevices& devices);
	~VulkanUniformBuffer();
	
	void create(mvpUniform data);
	void update(VulkanCommandPool *commandPool, uint32_t currentImage, mvpUniform newData);

	std::vector<VkBuffer> getBuffers() { return uniformBuffers; }
protected:
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	
	VulkanDevices				devices;
	std::vector<VkBuffer>		uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	VkBuffer					buffer;
	VkDeviceMemory				bufferMemory;
	mvpUniform					data;
};

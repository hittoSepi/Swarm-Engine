#pragma once
#include "Core/Vulkan/SwarmVulkan.h"

class VulkanSimpleRenderPass: public VulkanRenderPass
{
public:
	VulkanSimpleRenderPass(VulkanDevices& devices_);
	void quit() override
	{
		vkDestroyRenderPass(devices.vkDevice, renderPass, nullptr);
		
	}
protected:
	VulkanDevices& devices;
};


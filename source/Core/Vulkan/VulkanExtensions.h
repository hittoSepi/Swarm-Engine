#pragma once


class VulkanExtensions
{
public:
	static std::vector<const char*> getVulkanExtensions(std::vector<const char*> requiredExtensions);
};

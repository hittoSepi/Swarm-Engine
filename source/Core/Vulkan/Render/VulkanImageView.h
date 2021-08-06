#pragma once

#include "Utils/Debug/Debug.h"
/*

	VulkanImageView::CreateOptions createOptions(
			api->getSurface(),
			api->getVkDevice(),
			api->getPhysicalDevice(),
			nullptr);
		
	auto imgView = new VulkanImageView(createOptions)
*/
class VulkanImage
{
public:
	VulkanImage();

protected:

	VkImage image;
};


class VulkanImageView
{
public:
	
	// pack all variables here
	class CreateOptions
	{
	public:
		CreateOptions(
			VkDevice		device,
			VkImage			image,
			VkFormat		format,
			VkImageViewType	type):
			device(device),	image(image),
			format(format),	type(type)
		{
		}
		
		VkDevice		device;
		VkImage			image;
		VkFormat		format;
		VkImageViewType	type;
	};

	void release(VkDevice device)
	{
		vkDestroyImageView(options.device, imageView, nullptr);
	}

	~VulkanImageView()
	{
		vkDestroyImageView(options.device, imageView, nullptr);
	}

	VkImageView getImageView() { return imageView; }
	
	VulkanImageView(const CreateOptions& options):
		options(options)
	{
		VkImageViewCreateInfo	createInfo{};
		createInfo.sType		= VK_CI::SW_VULKAN_IMAGE_VIEW;
		createInfo.image		= options.image;
		createInfo.viewType		= options.type;
		createInfo.format		= options.format;
		createInfo.components.r = VK_SWIZZLE;
		createInfo.components.g = VK_SWIZZLE;
		createInfo.components.b = VK_SWIZZLE;
		createInfo.components.a = VK_SWIZZLE;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(options.device, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
			LogError(std::string("failed to create image views!"));
		}
	}
private:
	VulkanImageView():
		options(CreateOptions(nullptr, nullptr, VkFormat::VK_FORMAT_R8_SRGB, VK_IMAGE_VIEW_TYPE_2D))
	{
		if(options.device == nullptr)
		{
			LogError(std::string("Device is nullptr, abort."));
		}
	}
	
	CreateOptions options;
	VkImageView imageView;
};

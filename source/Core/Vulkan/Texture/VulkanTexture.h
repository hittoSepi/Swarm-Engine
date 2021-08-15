#pragma once



class VulkanTexture
{
public:
	VulkanTexture(const VulkanDevices& devices, std::string filename);
	~VulkanTexture();
	VkImage getTextureImage() { return textureImage; }

	void addToCommandPool(VulkanCommandPool* commandPool);
	
	int getWidth() {return texWidth; }
	int getHeight() {return texHeight; }

	iRect getSize() { return iRect(0, 0, texWidth, texHeight); }
	VkFormat getImageFormat() { return imageFormat; }
	VkImageCreateInfo getImageInfo() { return imageInfo; }
protected:
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createImage(
		VkImage& image,
		VkDeviceMemory& imageMemory,
		uint32_t width,
		uint32_t height,
		VkFormat format = VK_FORMAT_R8G8B8A8_SRGB,
		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
		VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	void transitionImageLayout(VulkanCommandPool* commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VulkanCommandPool* commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	
	VkImageCreateInfo imageInfo{};
	const VulkanDevices devices;
	std::string filename;
	int texWidth, texHeight, texChannels;
	VkImage textureImage;
	VkFormat imageFormat;
	VkDeviceMemory textureImageMemory;
		
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
};

class VulkanSampler
{
public:
	VulkanSampler(const VulkanDevices& devices, VulkanTexture* texture);
	~VulkanSampler();


	void load();

	VkImageView getTextureImageView() { return textureImageView; }
	VkSampler getTextureSampler() { return textureSampler; }

protected:

	VkImageView createImageView(const VkImage& image, const VkFormat& format, const VkImageViewType& imageType = VK_IMAGE_VIEW_TYPE_2D);

	VkImageViewType imageType;
	const VulkanDevices devices;
	VkImageView textureImageView;
	VkSampler textureSampler;
	
};
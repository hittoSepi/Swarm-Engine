#pragma once



class VulkanRasterizer
{
public:

	class CreateOptions
	{
	public:
		VkPipelineRasterizationStateCreateInfo createInfo {
			VK_CI::SW_VULKAN_PIPELINE_RASTERIZATION,
			nullptr,
			0,
			VK_FALSE,
			VK_FALSE,
			VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_CLOCKWISE,
			 VK_FALSE,
			1.0f, 1.0f, 1.0f
		};

		CreateOptions()
		{
			
		}
		
		CreateOptions(VkPipelineRasterizationStateCreateInfo ci)
		{
			createInfo = ci;
		}
	};

	VkPipelineRasterizationStateCreateInfo getCreateInfo() { return options.createInfo; }
	
	VulkanRasterizer()
	{
		options = CreateOptions();
		
	}
	
	VulkanRasterizer(const CreateOptions& options):
		options(options)
	{
				
	}
	
	CreateOptions options;
	
};
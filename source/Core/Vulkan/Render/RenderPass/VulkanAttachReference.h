#pragma once


/// <summary>
/// VulkanAttachmentReference
/// </summary>
class VulkanAttachmentReference
{
public:
	
	VulkanAttachmentReference(uint32_t refAttach, VkImageLayout layout)
	{
		reference.attachment  = refAttach;
		reference.layout = layout;
	}

	VkAttachmentReference getReference() { return reference; }
	VulkanAttachmentReference()
	{
	}
protected:
	
	VkAttachmentReference reference;
};

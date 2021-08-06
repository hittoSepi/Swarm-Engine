#include "pch.h"
#include <vulkan/vulkan.h>
#include "Core/Shader/ShaderHelpers.h"
#include "Core/Shader/ShaderCache.h"
#include "Core/Vulkan/Shader/VulkanShaderModule.h"

using namespace std;


VulkanShaderModule::VulkanShaderModule(VulkanDevice *device, Type type, string filename, string name):
	ShaderModule(device, type, filename, name)
{
}


VulkanShaderModule::~VulkanShaderModule()
{
}


void VulkanShaderModule::readSourceFile()
{
}


void VulkanShaderModule::destroy()
{
	VulkanDevice *dev = (VulkanDevice*)device;
	vkDestroyShaderModule((VkDevice)dev->getApiDevice(), module, nullptr);
}


void VulkanShaderModule::compile()
{
	auto cmd = "glslc.exe " + filename + " -o " + GetShaderCacheFile(this);
	LogInfo(cmd);
	
	// compile shader
	auto result = execA(cmd);

	// read compiled shader binary from file to create info
	vector<char> src = ReadBinaryFile(GetShaderCacheFile(this).c_str());

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_CI::SW_VULKAN_SHADER_MODULE;
	createInfo.codeSize = src.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(src.data());

	// create shader module
	VulkanDevice *dev = (VulkanDevice*)device;
	if (vkCreateShaderModule((VkDevice)dev->getApiDevice(), &createInfo, nullptr, &module) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	LogVerbose("OK.");
}




size_t HashShaderString(const char* pRootDir, const char* pShader, size_t hash)
{
	hash = Hash(pShader, strlen(pShader), hash);

	const char* pch = pShader;
	while (*pch != 0)
	{
		if (*pch == '/') // parse comments
		{
			pch++;
			if (*pch != 0 && *pch == '/')
			{
				pch++;
				while (*pch != 0 && *pch != '\n')
					pch++;
			}
			else if (*pch != 0 && *pch == '*')
			{
				pch++;
				while ((*pch != 0 && *pch != '*') && (*(pch + 1) != 0 && *(pch + 1) != '/'))
					pch++;
			}
		}
		else if (*pch == '#') // parse #include
		{
			pch++;
			const char include[] = "include";
			int i = 0;
			while ((*pch != 0) && *pch == include[i])
			{
				pch++;
				i++;
			}

			if (i == strlen(include))
			{
				while (*pch != 0 && *pch == ' ')
					pch++;

				if (*pch != 0 && *pch == '\"')
				{
					pch++;
					const char* pName = pch;

					while (*pch != 0 && *pch != '\"')
						pch++;

					char includeName[1024];
					strcpy_s<1024>(includeName, pRootDir);
					strncat_s<1024>(includeName, pName, pch - pName);

					pch++;

					const char* pShaderCode = ReadTextFile(includeName);
					if (strlen(pShaderCode) > 0)
					{
						hash = HashShaderString(pRootDir, pShaderCode, hash);
					}
				}
			}
		}
		else
		{
			pch++;
		}
	}

	return hash;
}

#include "pch.h"
#include "Core/Shader/Shader.h"
#include "Core/Vulkan/VulkanShader.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

using namespace std;

VkShaderStageFlagBits getBitFromType(ShaderModule::Type type)
{
	switch (type)
	{
	case ShaderModule::Type::VERTEX:
		return VK_SHADER_STAGE_VERTEX_BIT;
	case ShaderModule::Type::GEOMETRY:
		return VK_SHADER_STAGE_GEOMETRY_BIT;
	case ShaderModule::Type::FRAGMENT:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case ShaderModule::Type::COMPUTE:
		return VK_SHADER_STAGE_COMPUTE_BIT;
	case ShaderModule::Type::UNKNOWN:
		return VK_SHADER_STAGE_ALL_GRAPHICS;
	}
	return VK_SHADER_STAGE_ALL_GRAPHICS;
}

#define MAX_SHADER_STAGES 4

VulkanShader* VulkanShader::create(const CreateOptions& opts)
{
	VulkanShader* shader = new VulkanShader(opts);
	vector<string> files = GetFileListing(GetShaderLibraryDir(), opts.filename, "vert|frag|geom");

	LogInfo("Creating Shader: " << opts.name);
	VkPipelineShaderStageCreateInfo shaderStages[MAX_SHADER_STAGES];

	int stage = 0;
	for (auto file : files) {
		
		auto type = GetShaderModuleTypeEnum(file);
		
		VulkanShaderModule* module = new VulkanShaderModule((VulkanDevice*)opts.device, type, file, opts.name + "-" + GetShaderTypeStr(type));

		LogInfo("Compiling ShaderModule: " << module->getName());

		module->compile();
		shader->addModule(module);

		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage	= getBitFromType(module->getType());
		shaderStageInfo.module	= module->getModule();
		shaderStageInfo.pName	= "main";
		shaderStages[stage]		= shaderStageInfo;
		stage++;
	}

	return shader;
}


VulkanShader::VulkanShader(const CreateOptions& opts) :
	Shader(opts)
{
}


VulkanShader::~VulkanShader()
{
	LogInfo("Destroying Shader: " + getName());
	for (auto m : modules)
	{
		auto p = (VulkanShaderModule*)m;
		p->destroy();
	}
	modules.clear();
}


void VulkanShader::addModule(ShaderModule* module)
{
	modules.push_back(module);
}


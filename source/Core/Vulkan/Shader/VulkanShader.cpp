#include "pch.h"
#include "Core/Shader/Shader.h"
#include "Core/Vulkan/Shader/VulkanShader.h"

/*
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
*/

using namespace std;


VkShaderStageFlagBits getBitFromType(ShaderModule::Type type)
{
	switch (type)
	{
		case ShaderModule::Type::VERTEX:
			return VK_SHADER::VERTEX;
		case ShaderModule::Type::GEOMETRY:
			return VK_SHADER::GEOMETRY;
		case ShaderModule::Type::FRAGMENT:
			return VK_SHADER::FRAGMENT;
		case ShaderModule::Type::COMPUTE:
			return VK_SHADER_STAGE_COMPUTE_BIT;
		case ShaderModule::Type::UNKNOWN:
		default:
			return VK_SHADER::ALL;
	}
}


VulkanShader* VulkanShader::create(const CreateOptions& opts)
{
	VulkanShader* shader = new VulkanShader(opts);
	vector<string> files = GetFileListing(GetShaderLibraryDir(), opts.filename, "vert|frag|geom");

	LogInfo("Creating Shader: " << opts.name);

	for (auto file : files) {
		shader->addModule(file);
	}

	return shader;
}


VulkanShader::VulkanShader(const CreateOptions& opts) :
	Shader(opts)
{
	stageCount = 0;
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
	if(stageCount < MAX_SHADER_STAGES) {
		modules.push_back(module);
		stageCount++;
	}
}


void VulkanShader::addModule(string file)
{
	if(stageCount < MAX_SHADER_STAGES) {
		auto type = GetShaderModuleTypeEnum(file);
		
		VulkanShaderModule* module = new VulkanShaderModule((VulkanDevice*)options.device, type, file, options.name + "-" + GetShaderTypeStr(type));

		LogInfo("Compiling ShaderModule: " << module->getName());

		module->compile();
		modules.push_back(module);

		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType			= VK_CI::SW_VULKAN_PIPELINE_SHADER_STAGE;
		shaderStageInfo.stage			= getBitFromType(module->getType());
		shaderStageInfo.module			= module->getModule();
		shaderStageInfo.pName			= "main";
		shaderStages[stageCount]		= shaderStageInfo;
		stageCount++;
	}
}


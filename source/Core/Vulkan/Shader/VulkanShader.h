#pragma once

#define MAX_SHADER_STAGES 4

class VulkanShader: public Shader
{
public:
	~VulkanShader() override;
	static VulkanShader* create(const CreateOptions &opts);
	void addModule(ShaderModule *module) override;
	void addModule(std::string file);
	VkPipelineShaderStageCreateInfo* getStageInfo() { return shaderStages; }
	int size() { return stageCount; }
protected:
	VulkanShader(const CreateOptions &opts);
	VkPipelineShaderStageCreateInfo shaderStages[MAX_SHADER_STAGES];

	int stageCount;
};

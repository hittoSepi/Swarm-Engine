#pragma once

class VulkanShader: public Shader
{
public:
	~VulkanShader() override;
	static VulkanShader* create(const CreateOptions &opts);
	void addModule(ShaderModule *module) override;
protected:
	VulkanShader(const CreateOptions &opts);
};

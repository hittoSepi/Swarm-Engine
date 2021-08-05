#pragma once

std::string GetShaderTypeStr(ShaderModule::Type type);
std::string GetShaderFileString(ShaderModule *shader);
std::string GetShaderCacheFile(ShaderModule *shader);
ShaderModule::Type GetShaderModuleTypeEnum(std::string input);
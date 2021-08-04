#pragma once
#include "Shader.h"

std::string getShaderTypeStr(Shader::Type type);
std::string getShaderFileString(Shader *shader);
std::string getShaderCacheFile(Shader *shader);

#include "pch.h"
#include <string>
#include <regex>
using namespace std;

inline static map<string, ShaderModule::Type> ShaderModuleTypeStringToEnum = map<string, ShaderModule::Type>
{
	{"vert", ShaderModule::Type::VERTEX		},
	{"geom", ShaderModule::Type::GEOMETRY	},
	{"frag", ShaderModule::Type::FRAGMENT	},
	{"comp", ShaderModule::Type::COMPUTE	}
};


ShaderModule::Type GetShaderModuleTypeEnum(std::string input)
{
	regex reg("(vert|frag|geom|comp)");
	smatch m;
	regex_search(input, m, reg);
	for(auto x: m)
	{
		return ShaderModuleTypeStringToEnum[x];
	}
	return ShaderModule::Type::UNKNOWN;
}


string GetShaderTypeStr(ShaderModule::Type type)
{
	switch (type)
	{
	case ShaderModule::Type::VERTEX:
		return "vert";
	case ShaderModule::Type::GEOMETRY:
		return "geom";
	case ShaderModule::Type::FRAGMENT:
		return "frag";
	case ShaderModule::Type::COMPUTE:
		return "comp";
	}
	return "undefined";
}


string GetShaderFileString(ShaderModule *shader)
{
	return GetShaderLibraryDir() + shader->getFileName();
}

string GetShaderCacheFile(ShaderModule *shader)
{
	return GetShaderCacheDir() + shader->getName() + ".spv";
}

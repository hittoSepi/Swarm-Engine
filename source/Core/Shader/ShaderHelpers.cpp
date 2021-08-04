#include "pch.h"
#include "Core/Shader/ShaderCache.h"
#include "Core/Shader/Shader.h"

using namespace std;


string getShaderTypeStr(Shader::Type type)
{
	switch (type)
	{
	case Shader::Type::VERTEX:
		return "vert";
	case Shader::Type::GEOMETRY:
		return "geom";
	case Shader::Type::FRAGMENT:
		return "frag";
	case Shader::Type::COMPUTE:
		return "comp";
	}
	return "undefined";
}

string getShaderFileString(Shader *shader)
{
	return GetShaderLibraryDir() + shader->getName() + "." + getShaderTypeStr(shader->getType());
}

string getShaderCacheFile(Shader *shader)
{
	return GetShaderCacheDir() + shader->getName() + "-" +  getShaderTypeStr(shader->getType())  + ".spv";
}

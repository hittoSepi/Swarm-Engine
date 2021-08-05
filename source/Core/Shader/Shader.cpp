#include "pch.h"
#include "Core/Shader/Shader.h"

void ShaderModule::setMask()
{
	switch (type)
	{
	case ShaderModule::Type::VERTEX:
		typeMask = VERTEX_SHADER_MASK;
		break;
	case ShaderModule::Type::GEOMETRY:
		typeMask = GEOMETRY_SHADER_MASK;
		break;
	case ShaderModule::Type::FRAGMENT:
		typeMask = FRAGMENT_SHADER_MASK;
		break;
	default:break;
	}
}


#pragma once

#define VERTEX_SHADER_MASK 1
#define GEOMETRY_SHADER_MASK 2
#define FRAGMENT_SHADER_MASK 4

#include <string>

class ShaderModule
{
public:
	enum class Type
	{
		VERTEX,
		GEOMETRY,
		FRAGMENT,
		COMPUTE,
		UNKNOWN,
	};

	ShaderModule(Device *device, Type type, std::string filename, std::string name) :
		device(device), type(type), filename(filename), name(name)
	{
		setMask();
	}

	virtual ~ShaderModule() = default;

	virtual void destroy() = 0;
	virtual void compile() = 0;
	virtual void readSourceFile() = 0;

	Type		getType() { return type; }
	std::string	getName() { return name; }
	std::string getFileName() { return filename; }
	std::string getTypeString() { }
	int			getTypeMask() { return typeMask; }


protected:
	void				setMask();
	
	Device				*device;
	int					typeMask;
	std::string			source;
	std::string			filename;
	std::string			name;
	Type				type;
};



class Shader
{
public:
	class CreateOptions
	{
	public:
		std::string name;
		std::string filename;
		Device		*device;
	};
	
	virtual ~Shader()
	{}
	
	virtual void addModule(ShaderModule* module) = 0;

	const std::string& getName() { return name; }

protected:
	Shader(const CreateOptions &options) :
		options(options), name(options.name)
	{
		
	}
	CreateOptions options;
	int shaderMask;
	std::string name;
	std::vector<ShaderModule*> modules;
};



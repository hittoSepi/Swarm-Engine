#pragma once

class Shader
{
public:

	enum class Type
	{
		VERTEX,
		GEOMETRY,
		FRAGMENT,
		COMPUTE,
	};
	
	Shader(Type type, std::string name):
		type(type), name(name)
	{
	}
	
	virtual ~Shader() = default;
	
	virtual void compile() = 0;
	virtual void readSourceFile() = 0;

	Type		getType() { return type; }
	std::string getName() { return name; }

protected:
	std::string	source;
	std::string name;
	Type		type;
};


#pragma once

#include "Core/Shader/Shader.h"
#include "Utils/Helpers.h"

class DefineList : public std::map<const std::string, std::string>
{
public:
    bool Has(const std::string &str) const 
    {
        return find(str) != end();
    }

    size_t Hash(size_t result = HASH_SEED) const
    {
        for (auto it = begin(); it != end(); it++)
        {
            result = ::HashString(it->first, result);
            result = ::HashString(it->second, result);
        }
        return result;
    }

    friend DefineList operator+(DefineList   def1,        // passing lhs by value helps optimize chained a+b+c
                                const DefineList & def2)  // otherwise, both parameters may be const references
    {
        for (auto it = def2.begin(); it != def2.end(); it++)
            def1[it->first] = it->second;
        return def1;
    }
};


class VulkanShader: public Shader
{
public:
	VulkanShader(Type type, std::string name);
	
	~VulkanShader() override;
	
	void compile() override;
	void readSourceFile() override;
 private:
};
#pragma once

#include <map>
#include "Shader.h"
#include <string>

typedef std::map<std::string, Shader*>::iterator shaderIterator;
typedef std::pair<std::string, Shader*> shaderEntry;

class ShaderManager
{
public:
	static ShaderManager* GetSingletonPtr() 
	{ 
		if (!instance)
		{
			instance = new ShaderManager();
		}
		return instance; 
	}
	Shader* operator[] (std::string name) {return shaders[name]; }
	void Add(Shader* shader)
	{
		if (!strcmp(shader->GetName(), ""))
		{
			char buf[16];
			sprintf(buf, "unnamed%d", index++);
			shader->SetName(buf);			
		}
		shaders.insert(shaderEntry(shader->GetName(), shader));
	}
	shaderIterator BeginIterator() { return shaders.begin(); }
	shaderIterator EndIterator() { return shaders.end(); }
	void CompileShaders()
	{
		for (shaderIterator it = BeginIterator(); it != EndIterator(); ++it)
		{
			if (!it->second->Compile())
				printf("%s", it->second->GetErrorLog());
			else
				printf("Shader: %s\nCompiled Successfully\n", it->second->GetName());
			
		}
	}
	void ReloadShaders()
	{
		for (shaderIterator it = BeginIterator(); it != EndIterator(); ++it)
		{
			if (!it->second->Reload())
				printf("%s", it->second->GetErrorLog());
			else
				printf("Shader: %s\nCompiledSuccessfully\n", it->second->GetName());
		}
	}
private:
	static ShaderManager* instance;
	std::map<std::string, Shader*> shaders;
	int index;
};

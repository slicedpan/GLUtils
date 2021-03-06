#pragma once

#include <map>
#include "Shader.h"
#include <GL/glew.h>
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
	static ShaderManager& GetSingleton()
	{
		return *(GetSingletonPtr());
	}
	ShaderManager() : current(0) {}
	Shader* operator[] (std::string name) {return shaders[name]; }
	void Add(Shader* shader)
	{
		if (!strcmp(shader->GetName(), ""))
		{
			char buf[32];
			sprintf(buf, "unnamed%d", index++);
			shader->SetName(buf);			
		}
		shaders.insert(shaderEntry(shader->GetName(), shader));
	}
	shaderIterator BeginIterator() { return shaders.begin(); }
	shaderIterator EndIterator() { return shaders.end(); }
	void CompileShaders()
	{
		printf("Compiling Shaders...");
		for (shaderIterator it = BeginIterator(); it != EndIterator(); ++it)
		{
			printf("\n\n%s:\n", it->second->GetName());
			if (!it->second->Compile())
				printf("%s", it->second->GetErrorLog());		
			else
				printf("Compiled Successfully\n", it->second->GetName());
			
		}
	}
	void ReloadShaders()
	{
		for (shaderIterator it = BeginIterator(); it != EndIterator(); ++it)
		{
			if (!it->second->Reload())
			{
				printf("\n\n%s:\n", it->second->GetName());
				printf("%s", it->second->GetErrorLog());
			}
			else
				printf("Shader: %s\nCompiled Successfully\n", it->second->GetName());
		}
	}
	void ShaderActive(Shader* shader)
	{
		current = shader;
	}
	Shader* GetCurrent()
	{
		return current;
	}
	void UseFF()
	{
		current = 0;
		glUseProgram(0);
	}
private:
	static ShaderManager* instance;
	std::map<std::string, Shader*> shaders;
	Shader* current;
	int index;
};


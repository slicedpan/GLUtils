#pragma once

#include <cstring>
#include <map>
#include <string>
#include "Uniform.h"
#include "MiscUtils.h"

typedef std::pair<std::string, Uniform> uniformEntry;
typedef std::map<std::string, Uniform>::iterator uniformIterator;
typedef std::map<std::string, Uniform> UniformList;

class Shader
{
public:
	Shader(char * vertexFileName, char * fragmentFileName, char * shaderName);
	Shader();
	Shader(char * shaderName);
	void SetSource(char* vertexSource, char* fragmentSource);
	MapWrapper<std::string, Uniform> Uniforms;
	const UniformList& GetActiveUniforms() { return uniforms; }
	void SetSourceFiles(char * vertexFileName, char * fragmentFileName);
	~Shader(void);
	bool Compile();
	char * GetErrorLog() { return errorLog;	}
	unsigned int GetId() { return glID; }
	void Use();
	bool Reload();
	char * GetName() { return name; }
	void SetName(char* name) { strncpy(this->name, name, 64); }
	//int GetNumberOfUniforms() { return uniformNumber; }	use Uniforms.size();
private:
	std::map<std::string, Uniform> uniforms;
	unsigned int glID;
	unsigned int vertexID;
	unsigned int fragmentID;
	char errorLog[1024];
	void LoadFromFiles();
	void SetUniforms();
	void Init();
	char * vFileName;
	char * fFileName;
	char name[64];
	int uniformNumber;
	static Uniform dummy;
	void Register();
};

char * getSourceFromFile(char* filename);


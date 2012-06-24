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
	Shader(const char * vertexFileName, const char * fragmentFileName, const char * shaderName);   //deprecated
	Shader();
	Shader(const char * shaderName);
	void SetSource(const char* vertexSource, const char* fragmentSource);
	MapWrapper<std::string, Uniform> Uniforms;
	const UniformList& GetActiveUniforms() { return uniforms; }
	void SetSourceFiles(const char * vertexFileName, const char * fragmentFileName);
	~Shader(void);
	bool Compile();
	char * GetErrorLog() { return errorLog;	}
	unsigned int GetId() { return glID; }
	void Use();
	bool Load();
	bool Reload() { return Load(); } //deprecated
	char * GetName() { return name; }
	void SetName(const char* name) { strncpy(this->name, name, 64); }
	//int GetNumberOfUniforms() { return uniformNumber; }	use Uniforms.size();
private:
	std::map<std::string, Uniform> uniforms;
	unsigned int glID;
	unsigned int vertexID;
	unsigned int fragmentID;
	char errorLog[1024];
	bool LoadFromFiles();
	void SetUniforms();
	void Init();
	std::string vertexFileName;
	std::string fragmentFileName;
	char name[64];
	int uniformNumber;
	static Uniform dummy;
	void Register();
	std::string fragmentSource;
	std::string vertexSource;
	static int counter;
	bool compiled;
public:
	const bool& Compiled;
};

char * getSourceFromFile(const char* filename);


#include "Shader.h"
#include "Uniform.h"
#include "ShaderManager.h"
#include <GL\glew.h>
#include <cstdio>
#include <cstdlib>

Uniform Shader::dummy;

void Shader::Init()
{
	vFileName = 0;
	fFileName = 0;
}

void Shader::Register()
{
	ShaderManager::GetSingletonPtr()->Add(this);
}

Shader::Shader()
	 : Uniforms(uniforms),
	uniformNumber(0)
{	
	SetName("");
	Register();
}

Shader::Shader(char* name)	
	: Uniforms(uniforms),
	uniformNumber(0)
{
	SetName(name);
	Register();
}

Shader::Shader(char* vertexFileName, char* fragmentFileName, char* shaderName)
	 : Uniforms(uniforms),
	uniformNumber(0)
{
	SetName(shaderName);
	SetSourceFiles(vertexFileName, fragmentFileName);
	LoadFromFiles();
	Register();
}

void Shader::SetSource(char* vertexSource, char* fragmentSource)
{
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexID, 1, (const GLchar**)&vertexSource, 0);
	glShaderSource(fragmentID, 1, (const GLchar**)&fragmentSource, 0);
}

void Shader::SetSourceFiles(char * vertexFileName,char * fragmentFileName)
{
	vFileName = (char*)malloc(strlen(vertexFileName));
	fFileName = (char*)malloc(strlen(fragmentFileName));
	strcpy(vFileName, vertexFileName);
	strcpy(fFileName, fragmentFileName);
}

void Shader::LoadFromFiles()
{		
	vertexID = glCreateShader(GL_VERTEX_SHADER);	
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	char * vertexSource = getSourceFromFile(vFileName);
	char * fragmentSource = getSourceFromFile(fFileName);
	if (!vertexSource || !fragmentSource)
	{
		printf("Could not find files %s and %s!", vFileName, fFileName);
		return;
	}
	glShaderSource(vertexID, 1, (const GLchar**)&vertexSource, 0);
	glShaderSource(fragmentID, 1, (const GLchar**)&fragmentSource, 0);
	
	free(vertexSource);
	free(fragmentSource);	
}

void Shader::SetUniforms()
{
	char buf[64];
	int size;
	GLenum type;
	glGetProgramiv(glID, GL_ACTIVE_UNIFORMS, &uniformNumber);
	uniforms.clear();
	for (unsigned int i = 0; i < uniformNumber; ++i)
	{
		glGetActiveUniform(glID, i, 64, 0, &size, &type, buf);
		uniforms.insert(uniformEntry(std::string(buf), Uniform(std::string(buf), i)));
	}
}

bool Shader::Compile()
{
	char buf[256];
	int flag = 0;

	memset((void*)errorLog, 0, sizeof(char) * 1024);

	glID = glCreateProgram();

	sprintf(buf, "VertexShader:\n");
	strncat(errorLog, buf, strlen(buf));

	glCompileShader(vertexID);
	glGetShaderInfoLog(vertexID, 256, NULL, buf);
	strncat(errorLog, buf, 256);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &flag);
	if (!flag)
		return false;

	sprintf(buf, "FragmentShader:\n");
	strncat(errorLog, buf, strlen(buf));
	
	glCompileShader(fragmentID);
	glGetShaderInfoLog(fragmentID, 256, NULL, buf);
	strncat(errorLog, buf, 256);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &flag);
	if (!flag)
		return false;

	sprintf(buf, "Link:\n");
	strncat(errorLog, buf, strlen(buf));

	glAttachShader(glID, vertexID);
	glAttachShader(glID, fragmentID);
	glLinkProgram(glID);
	glGetProgramInfoLog(glID, 256, NULL, buf);	
	strncat(errorLog, buf, 256);
	glGetProgramiv(glID, GL_LINK_STATUS, &flag);
	if (!flag)
		return false;
	SetUniforms();
	return true;
}

Shader::~Shader(void)
{
}

char * getSourceFromFile(char* filename)
{
	FILE * input = 0;
	input = fopen(filename, "rb");
	if (!input)
		return 0;
	int size;
	char * buf;
	if (!input)
		return 0;
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);
	buf = (char*)malloc(sizeof(char) * (size + 1));
	fread(buf, sizeof(char), size, input);
	buf[size] = 0;
	fclose(input);
	return buf;
}

void Shader::Use()
{
	glUseProgram(glID);
	ShaderManager::GetSingletonPtr()->ShaderActive(this);
}

bool Shader::Reload()
{
	glDeleteProgram(glID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (vFileName && fFileName)
		LoadFromFiles();
	return Compile();
}

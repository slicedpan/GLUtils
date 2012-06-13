#include "Shader.h"
#include "Uniform.h"
#include "ShaderManager.h"
#include <GL\glew.h>
#include <cstdio>
#include <cstdlib>

Uniform Shader::dummy;
int Shader::counter = 0;

void Shader::Init()
{
	glID = 0;
	vertexID = 0;
	fragmentID = 0;
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
	//Register();
}

Shader::Shader(const char* name)	
	: Uniforms(uniforms),
	uniformNumber(0)
{
	SetName(name);
	//Register();
}

Shader::Shader(const char* vertexFileName, const char* fragmentFileName, const char* shaderName)
	 : Uniforms(uniforms),
	uniformNumber(0)
{
	SetName(shaderName);
	SetSourceFiles(vertexFileName, fragmentFileName);
	LoadFromFiles();
	//Register();
}

void Shader::SetSource(const char* vertexSource, const char* fragmentSource)
{
	this->fragmentSource.assign(fragmentSource);
	this->vertexSource.assign(vertexSource);

	vertexFileName.clear();
	fragmentFileName.clear();

}

void Shader::SetSourceFiles(const char * vertexFileName, const char * fragmentFileName)
{
	this->vertexFileName.assign(vertexFileName);
	this->fragmentFileName.assign(fragmentFileName);
}

bool Shader::LoadFromFiles()
{		
	vertexID = glCreateShader(GL_VERTEX_SHADER);	
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	char* vs = getSourceFromFile(vertexFileName.c_str());
	char* fs = getSourceFromFile(fragmentFileName.c_str());
	
	if (!fs || !vs)
	{
		printf("Could not find file(s): ");
		if (fs)
			free(fs);		
		else
			printf("%s", fragmentFileName.c_str());

		if ((!vs) && (!fs))
			printf(", ");

		if (vs)
			free(vs);
		else
			printf("%s", vertexFileName.c_str());

		return false;
	}
	vertexSource.assign(vs);
	fragmentSource.assign(fs);
	const GLchar* vertexPtr = vertexSource.c_str();
	const GLchar* fragPtr = fragmentSource.c_str();

	glShaderSource(vertexID, 1, &vertexPtr, 0);
	glShaderSource(fragmentID, 1, &fragPtr, 0);

	free(vs);
	free(fs);

	return true;

}

void Shader::SetUniforms()
{
	char buf[256];
	int size;
	GLenum type;
	glGetProgramiv(glID, GL_ACTIVE_UNIFORMS, &uniformNumber);
	uniforms.clear();
	for (unsigned int i = 0; i < uniformNumber; ++i)
	{
		glGetActiveUniform(glID, i, 256, 0, &size, &type, buf);
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

char * getSourceFromFile(const char* filename)
{
	FILE * input = 0;
	input = fopen(filename, "rb");
	if (!input)
		return 0;
	int size;
	char * buf;
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

bool Shader::Load()
{
	if (glID)
	{
		glDeleteProgram(glID);
		glID = 0;
	}
	if (vertexID)
	{
		glDeleteShader(vertexID);
		vertexID = 0;
	}
	if (fragmentID)
	{
		glDeleteShader(fragmentID);
		fragmentID = 0;
	}
	if (!vertexFileName.empty() && !fragmentFileName.empty())
	{
		if (!LoadFromFiles())
			return false;
	}
	return Compile();
}

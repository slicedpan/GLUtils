#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

struct FBOTexture;

class FrameBufferObject
{
public:
	FrameBufferObject(int width, int height, int depthBufferBitDepth, int stencilBufferBitDepth, GLenum textureFormat, GLenum textureType);
	~FrameBufferObject();
	void AttachTexture(std::string textureName);
	GLuint GetTexture(std::string name);
	GLuint GetTexture(GLuint index);
	bool CheckCompleteness();
	void Bind();
	void Unbind();
private:
	std::vector<FBOTexture*> textures;
	std::vector<GLuint> renderBuffers;
	GLuint glID;
	int width;
	int height;
	GLenum textureFormat;
	GLenum textureType;
};


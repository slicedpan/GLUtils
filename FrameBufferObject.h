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
	void AttachTexture(std::string textureName, GLenum magFilter, GLenum minFilter);
	GLuint GetTexture(std::string name);
	GLuint GetTexture(GLuint index);
	bool CheckCompleteness();
	void Bind();
	void Unbind();
	static void SetResetViewportSize(bool active);
	static void SetDefaultViewportSize(int height, int width);
private:
	std::vector<FBOTexture*> textures;
	std::vector<GLuint> renderBuffers;
	GLuint glID;
	int width;
	int height;
	GLenum textureFormat;
	GLenum textureType;
	unsigned int texNum;
	static int screenWidth;
	static int screenHeight;
	static bool resetViewport;
};


#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

struct FBOTexture
{
	GLuint glID;
	std::string name;
};


class FrameBufferObject
{
public:
	FrameBufferObject(int width, int height, int depthBufferBitDepth, int stencilBufferBitDepth, GLenum textureFormat, GLenum textureType, std::string name = "");
	~FrameBufferObject();
	void AttachTexture(std::string textureName, GLenum magFilter = GL_LINEAR, GLenum minFilter = GL_LINEAR);
	void AttachDepthTexture(std::string textureName, GLenum magFilter = GL_LINEAR, GLenum minFilter = GL_LINEAR, GLenum depthFormat = GL_DEPTH_COMPONENT);
	GLuint GetTextureID(std::string name);
	GLuint GetTextureID(GLuint index);
	FBOTexture* GetTexture(std::string name);
	FBOTexture* GetTexture(GLuint index);
	bool CheckCompleteness();
	void Bind();
	void Unbind();
	void SetDrawBuffers(bool active);
	static void SetResetViewportSize(bool active);
	static void SetDefaultViewportSize(int height, int width);
	std::string& GetName() { return name; }
	unsigned int GetNumTextures() { return textures.size(); }
private:
	void AttachTextureTo(std::string textureName, GLenum magFilter, GLenum minFilter, GLenum attachmentPoint, GLenum textureFormat, GLenum extFormat = GL_RGBA);
	std::vector<FBOTexture*> textures;
	std::vector<GLuint> renderBuffers;
	std::string name;
	GLuint glID;
	int width;
	int height;
	unsigned int nextIndex;
	GLenum textureFormat;
	GLenum textureType;
	unsigned int texNum;
	static int screenWidth;
	static int screenHeight;
	static bool resetViewport;
	bool setDrawBuffers;
};


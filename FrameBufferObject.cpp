#include "FrameBufferObject.h"
#include "FBOTexture.h"

FrameBufferObject::FrameBufferObject(int width, int height, int depthBufferBitDepth, int stencilBufferBitDepth, GLenum textureFormat, GLenum textureType)
	: width(width),
	height(height),
	textureFormat(textureFormat),
	textureType(textureType)
{
	glGenFramebuffers(1, &glID);
	if (!glGenFramebuffers)
		throw;
	if (depthBufferBitDepth || stencilBufferBitDepth)
	{
		//TODO add depth/stencil buffers
	}
}

FrameBufferObject::~FrameBufferObject(void)
{
}

void FrameBufferObject::AttachTexture(std::string name)
{
	FBOTexture* tex = new FBOTexture();
	tex->name = name;
	glGenTextures(1, &tex->glID);
	glBindTexture(textureType, tex->glID);
	glTexImage2D(textureType, 0, textureFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	GLuint attachPoint = GL_COLOR_ATTACHMENT0 + textures.size();
	glBindFramebuffer(GL_FRAMEBUFFER, glID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachPoint, GL_TEXTURE_2D, tex->glID, 0);
	textures.push_back(tex);
	glBindTexture(textureType, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferObject::GetTexture(std::string name)
{
	for (int i = 0; i < textures.size(); ++i)
	{
		if (!name.compare(textures[i]->name))
			return textures[i]->glID;
	}
	return 0;
}

GLuint FrameBufferObject::GetTexture(GLuint index)
{
	return textures[index]->glID;
}

bool FrameBufferObject::CheckCompleteness()
{
	glBindFramebuffer(GL_FRAMEBUFFER, glID);
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return (result == GL_FRAMEBUFFER_COMPLETE);
}

void FrameBufferObject::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, glID);
}

void FrameBufferObject::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

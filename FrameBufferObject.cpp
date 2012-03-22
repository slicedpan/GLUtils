#include "FrameBufferObject.h"
#include "FBOTexture.h"

FrameBufferObject::FrameBufferObject(int width, int height, int depthBufferBitDepth, int stencilBufferBitDepth, GLenum textureFormat, GLenum textureType)
	: width(width),
	height(height),
	textureFormat(textureFormat),
	textureType(textureType),
	texNum(0)
{
	if (!glGenFramebuffers)
		throw;
	glGenFramebuffers(1, &glID);
	glBindFramebuffer(GL_FRAMEBUFFER, glID);
	if (depthBufferBitDepth || stencilBufferBitDepth)
	{
		GLenum format = 0;
		if (depthBufferBitDepth && stencilBufferBitDepth)
		{
			if (depthBufferBitDepth == 32 && stencilBufferBitDepth == 8)
				format = GL_DEPTH32F_STENCIL8;
			else
				format = GL_DEPTH24_STENCIL8;
			GLuint id;
			glGenRenderbuffers(1, &id);
			renderBuffers.push_back(id);
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
		}
		else if (stencilBufferBitDepth == 0)
		{
			if (depthBufferBitDepth == 16)			
				format = GL_DEPTH_COMPONENT16;
			else if (depthBufferBitDepth == 24)
				format = GL_DEPTH_COMPONENT24;
			else
				format = GL_DEPTH_COMPONENT32F;
			GLuint id;
			glGenRenderbuffers(1, &id);
			renderBuffers.push_back(id);
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);			
			
		}		
	}
	int depthBits, stencilBits;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &depthBits);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &stencilBits);
	printf("depthBits: %d, stencilBits %d\n", depthBits, stencilBits);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GLuint attachPoint = GL_COLOR_ATTACHMENT0 + texNum++;
	glBindFramebuffer(GL_FRAMEBUFFER, glID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachPoint, GL_TEXTURE_2D, tex->glID, 0);
	textures.push_back(tex);
	glBindTexture(textureType, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!CheckCompleteness())
		throw;
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
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		switch(result)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			printf("Framebuffer undefined!\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			printf("Framebuffer incomplete attachment!\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			printf("Framebuffer incomplete, missing attachment!\n");
			break;
		default:
			printf("Framebuffer incomplete!\n");
			break;
		}
	}
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

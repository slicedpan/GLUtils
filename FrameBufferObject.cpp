#include "FrameBufferObject.h"
#include "FBOManager.h"

int FrameBufferObject::screenHeight= -1;
int FrameBufferObject::screenWidth = -1;
bool FrameBufferObject::resetViewport = true;

FrameBufferObject::FrameBufferObject(int width, int height, int depthBufferBitDepth, int stencilBufferBitDepth, GLenum textureFormat, GLenum textureType, std::string name)
	: width(width),
	height(height),
	textureFormat(textureFormat),
	textureType(textureType),
	texNum(0),
	nextIndex(0),
	Height(height),
	Width(width),
	Bound(bound)
{
	if (!glGenFramebuffers)
		throw;

	bound = false;
	setDrawBuffers = false;

	if (name == "")
	{
		char buf[64];
		sprintf(buf, "unnamed%d", ++nextIndex);
		this->name = buf;
	}
	else
	{
		this->name = name;
	}

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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

FrameBufferObject::~FrameBufferObject(void)
{
	glDeleteFramebuffers(1, &glID);
}

void FrameBufferObject::SetDrawBuffers(bool active)
{
	setDrawBuffers = active;
}

void FrameBufferObject::AttachDepthTexture(std::string name, GLenum magFilter, GLenum minFilter, GLenum depthFormat)
{
	AttachTextureTo(name, magFilter, minFilter, GL_DEPTH_ATTACHMENT, depthFormat, GL_DEPTH_COMPONENT);
}

void FrameBufferObject::AttachTextureTo(std::string name, GLenum magFilter, GLenum minFilter, GLenum attachmentPoint, GLenum texFormat, GLenum extFormat)
{
	FBOTexture* tex = new FBOTexture();
	tex->name = name;
	glGenTextures(1, &tex->glID);
	glBindTexture(textureType, tex->glID);
	glTexImage2D(textureType, 0, texFormat, width, height, 0, extFormat, GL_FLOAT, 0);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minFilter);
	glBindFramebuffer(GL_FRAMEBUFFER, glID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, tex->glID, 0);
	textures.push_back(tex);
	glBindTexture(textureType, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::AttachTexture(std::string name, GLenum minFilter, GLenum magFilter)
{
	AttachTextureTo(name, magFilter, minFilter, GL_COLOR_ATTACHMENT0 + texNum++, textureFormat);
}

FBOTexture* FrameBufferObject::GetTexture(std::string name)
{
	for (int i = 0; i < textures.size(); ++i)
	{
		if (!name.compare(textures[i]->name))
			return textures[i];
	}
	return 0;
}

FBOTexture* FrameBufferObject::GetTexture(GLuint index)
{
	return textures[index];
}

GLuint FrameBufferObject::GetTextureID(std::string name)
{
	return GetTexture(name)->glID;
}

GLuint FrameBufferObject::GetTextureID(GLuint index)
{
	return GetTexture(index)->glID;
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
	if (screenWidth < 0 && screenHeight < 0)
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		screenWidth = viewport[2];
		screenHeight = viewport[3];
	}	
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, glID);
	if (setDrawBuffers)
	{
		GLenum* buffers;
		buffers = (GLenum*)malloc(sizeof(GLenum) * texNum);
		for (int i = 0; i < texNum; ++i)
		{
			buffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(texNum, buffers);
	}
	bound = true;
}

void FrameBufferObject::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	if (setDrawBuffers)
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	bound = false;
}

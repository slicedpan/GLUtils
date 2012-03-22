#include "BasicTexture.h"
#include "stb_image.h"
#include <GL\glew.h>

BasicTexture::BasicTexture(char* filename) 
	: imgData(0),
	glID(0)
{
	strncpy(this->filename, filename, strlen(this->filename));
}

BasicTexture::~BasicTexture(void)
{
}

void BasicTexture::LoadFromFile()
{
	FILE* file = fopen(this->filename, "rb");
	if (!file)
		return;
	imgData = stbi_load_from_file(file, &width, &height, &components, 4);
	fclose(file);
	glGenTextures(1, &glID);
	glBindTexture(GL_TEXTURE_2D, glID);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glEnable(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void BasicTexture::Load()
{
	if (glID > 0)
		glDeleteTextures(1, &glID);
	glID = 0;
	LoadFromFile();
}

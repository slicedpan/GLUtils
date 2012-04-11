#include "BasicTexture.h"
#include "stb_image.h"
#include <cstdlib>
#include <GL\glew.h>

BasicTexture::BasicTexture(char* filename) 
	: imgData(0),
	glID(0)
{
	this->filename = (char*)malloc(strlen(filename));
	strcpy(this->filename, filename);
}

BasicTexture::~BasicTexture(void)
{
}

void BasicTexture::LoadFromFile()
{
	printf("opening file: %s\n", this->filename);
	FILE* file = fopen(this->filename, "rb");
	if (!file)
	{
		printf("couldn't open file: %s\n", this->filename);
		return;
	}
	printf("%s opened\n", this->filename);
	imgData = stbi_load_from_file(file, &width, &height, &components, 4);
	printf("loaded data from file\n");
	glGenTextures(1, &glID);
	printf("generated texture name\n");
	glBindTexture(GL_TEXTURE_2D, glID);
	printf("bound texture to opengl context\n");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	printf("loaded texture to GPU\n");
	glEnable(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);
	printf("generated mipmaps\n");
	stbi_image_free(imgData);
	printf("freed image data\n");
	fclose(file);
	printf("closed file\n");
	glBindTexture(GL_TEXTURE_2D, 0);
}

void BasicTexture::Load()
{
	if (glID > 0)
		glDeleteTextures(1, &glID);
	glID = 0;
	LoadFromFile();
}

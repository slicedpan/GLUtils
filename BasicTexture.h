#ifndef BASICTEXTURE_H
#define BASICTEXTURE_H

#include <cstring>

class BasicTexture
{
public:
	BasicTexture(char* filename);
	~BasicTexture(void);
	unsigned int GetId() { return glID; }
	void Load();
	const int& Width;
	const int& Height;
protected:
	char* filename;
	virtual void LoadFromFile();
	unsigned char* imgData;
	int height;
	int width;
	int components;
	unsigned int glID;
};

#endif


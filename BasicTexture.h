#pragma once
#include <cstring>

class BasicTexture
{
public:
	BasicTexture(char* filename);
	~BasicTexture(void);
	unsigned int GetId() { return glID; }
	void Load();
protected:
	char* filename;
	virtual void LoadFromFile();
	unsigned char* imgData;
	int height;
	int width;
	int components;
	unsigned int glID;
};


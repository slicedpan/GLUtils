#pragma once

class Vec2;

struct vertex
{
	float position[2];
	float texCoord[2];
};

class QuadDrawer
{
public:
	static void DrawQuad(Vec2& min, Vec2& max);
	static void DrawQuad(Vec2& min, Vec2& max, Vec2& pixSize);
	static void DrawQuads(int number);
	~QuadDrawer();
private:
	static bool initialised;
	static QuadDrawer* instance;
	static void Initialise();
	static void CleanUp();
	void CreateVBO();
	unsigned int vboID;
	unsigned int iboID;
	unsigned char * indices;
	vertex * vertexData;
	unsigned int vaoID;
	Vec2* zero;
};


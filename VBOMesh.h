#pragma once

class objLoader;

struct VertexPosition
{
	float position[3];
};

struct VertexPositionNormal
{
	float position[3];
	float normal[3];
};

struct VertexPositionNormalTexcoord
{
	float position[3];
	float normal[3];
	float texCoord[2];
};

struct VertexPositionTexcoord
{
	float position[3];
	float texCoord[2];
};

class VBOMesh
{
public:
	VBOMesh(char * filename, bool smoothNormals, bool generateNormals);
	~VBOMesh(void);
	bool HasNormals() { return hasNormals; }
	bool HasTextureCoords() { return hasTextureCoords; }
	unsigned int GetNumVertices() { return vertexCount; }
	unsigned int GetNumTriangles() { return triCount; }
	void Draw();
	void DrawImmediate();
	void Print();
	void Load();
	void CleanUp();
private:
	objLoader* obj;
	float* meshData;
	unsigned short* indexData;
	bool hasNormals;
	bool hasTextureCoords;
	bool generateNormals;
	char* filename;
	unsigned int bufID;
	unsigned int vertexCount;
	unsigned int triCount;
	unsigned int indexBufID;
	unsigned int vaoID;
	unsigned int vertexComponents;
	unsigned int vertexSize;
	bool loaded;
};


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

struct MeshInfo
{
	unsigned int vertexComponents;
	unsigned int vertexSize;
	unsigned int vertexCount;
	unsigned int triCount;
	unsigned int indexFormat;
	unsigned int componentFlags;
};

class VBOMesh
{
public:
	VBOMesh(char * filename, bool smoothNormals, bool generateNormals);
	~VBOMesh(void);
	bool HasNormals() { return hasNormals; }
	bool HasTextureCoords() { return hasTextureCoords; }
	unsigned int GetNumVertices() { return meshInfo.vertexCount; }
	unsigned int GetNumTriangles() { return meshInfo.triCount; }
	void Draw();
	void DrawImmediate();
	void Print();
	void Load();
	void CleanUp();
private:
	objLoader* obj;
	float* meshData;
	unsigned char* byteIndexData;
	unsigned short* indexData;
	unsigned int* longIndexData;
	bool hasNormals;
	bool hasTextureCoords;
	bool generateNormals;
	char* filename;
	MeshInfo meshInfo;
	unsigned int bufID;
	unsigned int indexBufID;
	unsigned int vaoID;
	bool loaded;
	bool LoadCached();
	void GenerateCache();
	void InitialiseVAO();
};


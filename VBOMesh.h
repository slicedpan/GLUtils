#pragma once

class objLoader;

#define HASNORMALS 1
#define HASTEXTURECOORDS 2

#define VF_POSITION 0
#define VF_POSITIONNORMAL HASNORMALS
#define VF_POSITIONTEXCOORD HASTEXTURECOORDS
#define VF_POSITIONNORMALTEXCOORD (HASNORMALS & HASTEXTURECOORDS)

struct Triangle
{
	unsigned int index[3];
};

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
	template <typename T> 
	T* GetVertexData(int index);
	Triangle GetTriangle(int triIndex);
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

template <typename T>
T* VBOMesh::GetVertexData(int index)
{
	char* ptr = (char*)meshData;
	ptr += meshInfo.vertexSize * index;
	return (T*)ptr;
}



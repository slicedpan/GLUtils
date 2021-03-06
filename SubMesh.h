#pragma once

class objLoader;
class Vec3;

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
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
};

struct SubMeshBlock
{
	size_t size;
	unsigned char* data;
};

class SubMesh
{
public:
	SubMesh();
	~SubMesh(void);
	bool HasNormals() { return meshInfo.componentFlags & HASNORMALS; }
	bool HasTextureCoords() { return meshInfo.componentFlags & HASTEXTURECOORDS; }
	unsigned int GetNumVertices() { return meshInfo.vertexCount; }
	unsigned int GetNumTriangles() { return meshInfo.triCount; }
	void Draw();
	void DrawImmediate();
	void Print();
	void LoadObj(objLoader* obj, bool smoothNormals, bool generateNormals);
	void CleanUp();
	void LoadCached(unsigned char* data);
	SubMeshBlock GenerateCache();
	template <typename T> 
	T* GetVertexData(int index);
	Triangle GetTriangle(int triIndex);
private:
	objLoader* obj;
	float* meshData;
	unsigned char* byteIndexData;
	unsigned short* indexData;
	unsigned int* longIndexData;	
	MeshInfo meshInfo;
	unsigned int bufID;
	unsigned int indexBufID;
	unsigned int vaoID;
	bool loaded;	
	void InitialiseVAO();
	Vec3* min;
	Vec3* max;
public:
	const Vec3& Min;
	const Vec3& Max;
};

template <typename T>
T* SubMesh::GetVertexData(int index)
{
	char* ptr = (char*)meshData;
	ptr += meshInfo.vertexSize * index;
	return (T*)ptr;
}



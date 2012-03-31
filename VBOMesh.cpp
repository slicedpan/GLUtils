#include "VBOMesh.h"
#include "ObjLoader/objLoader.h"
#include "DebugDraw.h"
#include <GL/glew.h>
#include <cstdlib>

#define HASNORMALS 1
#define HASTEXTURECOORDS 2

Vec3 FromFloatV(float * ptr)
{
	return Vec3(ptr[0], ptr[1], ptr[2]);
}

bool VBOMesh::LoadCached()
{
	char* cachedFilename = (char*)malloc(sizeof(char) * strlen(filename) + 7);
	sprintf(cachedFilename, "%s.cache", filename);
	printf("%s", cachedFilename);
	FILE* cacheFile = fopen(cachedFilename, "rb");
	if (!cacheFile)
		return false;

	fread(&meshInfo, sizeof(MeshInfo), 1, cacheFile);

	meshData = (float*)malloc(meshInfo.vertexCount * meshInfo.vertexSize);
	fread(meshData, meshInfo.vertexSize, meshInfo.vertexCount, cacheFile);

	switch (meshInfo.indexFormat)
	{
	case GL_UNSIGNED_INT:
		longIndexData = (unsigned int*)malloc(sizeof(unsigned int) * 3 * meshInfo.triCount);
		fread(longIndexData, sizeof(unsigned int), 3 * meshInfo.triCount, cacheFile);
		break;
	case GL_UNSIGNED_SHORT:
		indexData = (unsigned short*)malloc(sizeof(unsigned short) * 3 * meshInfo.triCount);
		fread(indexData, sizeof(unsigned short), 3 * meshInfo.triCount, cacheFile);
		break;
	case GL_UNSIGNED_BYTE:
		break;
	}

	hasNormals = meshInfo.componentFlags & HASNORMALS;
	hasTextureCoords = meshInfo.componentFlags & HASTEXTURECOORDS;

	InitialiseVAO();

	return false;
}

void VBOMesh::GenerateCache()
{
	char* cachedFilename = (char*)malloc(sizeof(char) * strlen(filename) + 7);
	sprintf(cachedFilename, "%s.cache", filename);
	printf("%s", cachedFilename);
	FILE* cacheFile = fopen(cachedFilename, "wb");
	fwrite(&meshInfo, sizeof(MeshInfo), 1, cacheFile);	//header

	fwrite(meshData, meshInfo.vertexSize, meshInfo.vertexCount, cacheFile);	//vertex data

	switch (meshInfo.indexFormat)	//index data
	{
	case GL_UNSIGNED_INT:
		fwrite(longIndexData, sizeof(unsigned int), meshInfo.triCount * 3, cacheFile);
		break;
	case GL_UNSIGNED_SHORT:
		fwrite(indexData, sizeof(unsigned short), meshInfo.triCount * 3, cacheFile);
		break;
	case GL_UNSIGNED_BYTE:
		break;
	}

}

void VBOMesh::Load()
{
	
	if (LoadCached())
		return;

	memset(&meshInfo, 0, sizeof(MeshInfo));

	obj = new objLoader();

	if (!obj->load(filename))
	{
		meshInfo.triCount = 0;
		meshInfo.vertexCount = 0;
		vaoID = 0;
		return;
	}	

	glGenVertexArrays(1, &vaoID);

	bool hasNormals = true;
	bool hasTextureCoords = true;

	for (int i = 0; i < obj->faceCount; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (obj->faceList[i]->normal_index[j] < 0)
				hasNormals = false;
			if (obj->faceList[i]->texture_index[j] < 0)
				hasTextureCoords = false;
		}
	}	

	if (hasTextureCoords && (hasNormals || generateNormals))
	{
		meshInfo.vertexSize = sizeof(VertexPositionNormalTexcoord);
		meshInfo.vertexComponents = 8;
	}
	else if (hasTextureCoords)
	{
		meshInfo.vertexSize = sizeof(VertexPositionTexcoord);
		meshInfo.vertexComponents = 5;
	}
	else if (hasNormals || generateNormals)
	{
		meshInfo.vertexSize = sizeof(VertexPositionNormal);
		meshInfo.vertexComponents = 6;
	}
	else
	{
		meshInfo.vertexSize = sizeof(VertexPosition);
		meshInfo.vertexComponents = 3;
	}
	
	meshInfo.vertexCount = obj->faceCount * 3;
	meshInfo.triCount = obj->faceCount;

	if (meshInfo.vertexCount > 65535)
	{
		longIndexData = (unsigned int*)malloc(sizeof(int) * meshInfo.triCount * 3);
		meshInfo.indexFormat = GL_UNSIGNED_INT;
	}
	else if (meshInfo.vertexCount > 255)
	{
		indexData = (unsigned short*)malloc(sizeof(unsigned short) * meshInfo.triCount * 3);
		meshInfo.indexFormat = GL_UNSIGNED_SHORT;
	}
	else
	{
		byteIndexData = (unsigned char*)malloc(sizeof(unsigned char) * meshInfo.triCount * 3);
		meshInfo.indexFormat = GL_UNSIGNED_BYTE;
	}

	meshData = (float*)malloc(meshInfo.vertexSize * meshInfo.vertexCount);	
	unsigned int meshDataIndex = 0;		
	unsigned int vertexOffset;
	int indexCount = 0;

	for (int i = 0; i < obj->faceCount; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			vertexOffset = 0;
			meshData[meshDataIndex] = obj->vertexList[obj->faceList[i]->vertex_index[j]]->e[0];
			meshData[meshDataIndex + 1] = obj->vertexList[obj->faceList[i]->vertex_index[j]]->e[1];
			meshData[meshDataIndex + 2] = obj->vertexList[obj->faceList[i]->vertex_index[j]]->e[2];

			vertexOffset += 3;

			if (hasNormals)
			{
				meshData[meshDataIndex + vertexOffset] = obj->normalList[obj->faceList[i]->normal_index[j]]->e[0];
				meshData[meshDataIndex + vertexOffset + 1] = obj->normalList[obj->faceList[i]->normal_index[j]]->e[1];
				meshData[meshDataIndex + vertexOffset + 2] = obj->normalList[obj->faceList[i]->normal_index[j]]->e[2];
				vertexOffset += 3;
			}

			if (hasTextureCoords)
			{
				meshData[meshDataIndex + vertexOffset] = obj->textureList[obj->faceList[i]->texture_index[j]]->e[0];
				meshData[meshDataIndex + vertexOffset + 1] = obj->textureList[obj->faceList[i]->texture_index[j]]->e[1];
				vertexOffset += 2;
			}
			meshDataIndex += meshInfo.vertexComponents;
			
			if (meshInfo.indexFormat == GL_UNSIGNED_INT)
			{
				longIndexData[(i * 3) + j] = indexCount++;
			}
			else
			{
				indexData[(i * 3) + j] = indexCount++;
			}
		}			
	}

	if (generateNormals && !hasNormals)
	{		
		Vec3 v1, v2, v3;
		for (int i = 0; i < obj->faceCount; ++i)
		{
			v1 = FromFloatV(meshData + (meshInfo.vertexComponents * i * 3));
			v2 = FromFloatV(meshData + (meshInfo.vertexComponents * (i * 3 + 1)));
			v3 = FromFloatV(meshData + (meshInfo.vertexComponents * (i * 3 + 2)));
			Vec3 normal = -norm(cross(v1 - v2, v2 - v3));
			memcpy(meshData + (meshInfo.vertexComponents * i * 3) + 3, normal.Ref(), sizeof(float) * 3);
		}
		hasNormals = true;
	}

	if (hasNormals)
		meshInfo.componentFlags |= HASNORMALS;
	if (hasTextureCoords)
		meshInfo.componentFlags |= HASTEXTURECOORDS;

	InitialiseVAO();	
	GenerateCache();
}

void VBOMesh::InitialiseVAO()
{
	glGenBuffers(1, &bufID);
	glGenBuffers(1, &indexBufID);
	
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufID);
	glBufferData(GL_ARRAY_BUFFER, meshInfo.vertexSize * meshInfo.vertexCount , meshData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufID);
	switch (meshInfo.indexFormat)
	{
	case GL_UNSIGNED_INT:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * meshInfo.triCount * 3, longIndexData, GL_STATIC_DRAW);
		break;
	case GL_UNSIGNED_SHORT:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshInfo.triCount * 3 * sizeof(unsigned short), indexData, GL_STATIC_DRAW);
		break;
	case GL_UNSIGNED_BYTE:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshInfo.triCount * 3 * sizeof(unsigned char), byteIndexData, GL_STATIC_DRAW);
		break;
	}
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, meshInfo.vertexSize, (void*)0);

	if (hasNormals)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, meshInfo.vertexSize, (void*)(3 * sizeof(float)));
		if (hasTextureCoords)
		{
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, meshInfo.vertexSize, (char*)0 + 6 * sizeof(float));
		}
	}
	else if (hasTextureCoords)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, meshInfo.vertexSize, (char*)0 + 3 * sizeof(float));
	}	

	glBindVertexArray(0);
	loaded = true;
}

VBOMesh::VBOMesh(char * filename, bool smoothNormals, bool generateNormals)
	: hasNormals(false),
	hasTextureCoords(false),
	generateNormals(generateNormals),
	filename(filename),
	meshData(0),
	indexData(0),
	vaoID(0),
	bufID(0),
	indexBufID(0),
	loaded(false)
{
	
}

void VBOMesh::CleanUp()
{
	if (meshData)
	{
		free(meshData);
		meshData = 0;
	}
	if (indexData)
	{
		free(indexData);
		indexData = 0;
	}
	if (vaoID)
	{
		glDeleteVertexArrays(1, &vaoID);
		vaoID = 0;
	}
	if (bufID)
	{
		glDeleteBuffers(1, &bufID);
		bufID = 0;
	}
	if (indexBufID)
	{
		glDeleteBuffers(1, &indexBufID);
		indexBufID = 0;
	}
	loaded = false;
}

VBOMesh::~VBOMesh(void)
{
	delete obj;
}

void VBOMesh::Draw()
{
	if (!loaded)
		return;
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, meshInfo.triCount * 3, meshInfo.indexFormat, (void*)0);
	glBindVertexArray(0);
}

void VBOMesh::DrawImmediate()
{
	if (!loaded)
		return;
	obj->Draw();
}

void VBOMesh::Print()
{
	for (int i = 0; i < meshInfo.triCount; ++i)
	{
		float* pos1 = meshData + (i * 3 * meshInfo.vertexComponents);
		printf("Tri: %d\n", i);
		printf("index: %d, p1: %f, %f, %f\t n1: %f, %f, %f\n", (i * 3), *pos1, *(pos1 + 1), *(pos1 + 2), *(pos1 + 3), *(pos1 + 4), *(pos1 + 5));
		pos1 += meshInfo.vertexComponents;
		printf("index: %d, p2: %f, %f, %f\t n2: %f, %f, %f\n", (i * 3) + 1, *pos1, *(pos1 + 1), *(pos1 + 2), *(pos1 + 3), *(pos1 + 4), *(pos1 + 5));
		pos1 += meshInfo.vertexComponents;
		printf("index: %d, p3: %f, %f, %f\t n3: %f, %f, %f\n", (i * 3) + 2, *pos1, *(pos1 + 1), *(pos1 + 2), *(pos1 + 3), *(pos1 + 4), *(pos1 + 5));
	}
	for (int i = 0; i < meshInfo.triCount; ++i)
	{
		printf("Tri: %d, indices: %d, %d, %d\n", i, indexData[(i * 3)], indexData[(i * 3) + 1], indexData[(i * 3) + 2]);
	}
}

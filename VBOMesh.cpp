#include "VBOMesh.h"
#include "ObjLoader/objLoader.h"
#include "DebugDraw.h"
#include <GL/glew.h>
#include <cstdlib>

Vec3 FromFloatV(float * ptr)
{
	return Vec3(ptr[0], ptr[1], ptr[2]);
}

void VBOMesh::Load()
{
	obj = new objLoader();

	if (!obj->load(filename))
	{
		triCount = 0;
		vertexCount = 0;
		vaoID = 0;
		return;
	}	

	glGenVertexArrays(1, &vaoID);

	hasNormals = true;
	hasTextureCoords = true;

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
		vertexSize = sizeof(VertexPositionNormalTexcoord);
		vertexComponents = 8;
	}
	else if (hasTextureCoords)
	{
		vertexSize = sizeof(VertexPositionTexcoord);
		vertexComponents = 5;
	}
	else if (hasNormals || generateNormals)
	{
		vertexSize = sizeof(VertexPositionNormal);
		vertexComponents = 6;
	}
	else
	{
		vertexSize = sizeof(VertexPosition);
		vertexComponents = 3;
	}
	
	vertexCount = obj->faceCount * 3;
	triCount = obj->faceCount;

	if (vertexCount > 65535)
	{
		longIndexData = (unsigned int*)malloc(sizeof(int) * triCount * 3);
		indexFormat = GL_UNSIGNED_INT;
	}
	else
	{
		indexData = (unsigned short*)malloc(sizeof(unsigned short) * triCount * 3);
		indexFormat = GL_UNSIGNED_SHORT;
	}

	meshData = (float*)malloc(vertexSize * vertexCount);	
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
			meshDataIndex += vertexComponents;
			
			if (indexFormat == GL_UNSIGNED_INT)
			{
				longIndexData[(i * 3) + j] = indexCount++;
			}
			else
			{
				indexData[(i * 3) + j] = indexCount++;
			}
		}			
	}

	if (triCount != obj->faceCount)
		throw;

	if (generateNormals && !hasNormals)
	{		
		Vec3 v1, v2, v3;
		for (int i = 0; i < obj->faceCount; ++i)
		{
			v1 = FromFloatV(meshData + obj->faceList[i]->vertex_index[0]);
			v2 = FromFloatV(meshData + obj->faceList[i]->vertex_index[1]);
			v3 = FromFloatV(meshData + obj->faceList[i]->vertex_index[2]);
			Vec3 normal = -norm(cross(v1 - v2, v2 - v3));
			memcpy(meshData + (vertexComponents * i * 3) + 3, normal.Ref(), sizeof(float) * 3);
		}
		hasNormals = true;
	}

	glGenBuffers(1, &bufID);
	glGenBuffers(1, &indexBufID);
	
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufID);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount , meshData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufID);
	if (indexFormat == GL_UNSIGNED_INT)	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * triCount * 3, longIndexData, GL_STATIC_DRAW);
	else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triCount * 3 * sizeof(unsigned short), indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);

	if (hasNormals)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (char*)0 + (3 * sizeof(float)));
	}

	glBindVertexArray(0);
	//TODO set vertex attributes for normals/texcoords
	//Print();
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
	glDrawElements(GL_TRIANGLES, triCount * 3, indexFormat, (void*)0);
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
	for (int i = 0; i < triCount; ++i)
	{
		float* pos1 = meshData + (i * 3 * vertexComponents);
		printf("Tri: %d\n", i);
		printf("index: %d, p1: %f, %f, %f\t n1: %f, %f, %f\n", (i * 3), *pos1, *(pos1 + 1), *(pos1 + 2), *(pos1 + 3), *(pos1 + 4), *(pos1 + 5));
		pos1 += vertexComponents;
		printf("index: %d, p2: %f, %f, %f\t n2: %f, %f, %f\n", (i * 3) + 1, *pos1, *(pos1 + 1), *(pos1 + 2), *(pos1 + 3), *(pos1 + 4), *(pos1 + 5));
		pos1 += vertexComponents;
		printf("index: %d, p3: %f, %f, %f\t n3: %f, %f, %f\n", (i * 3) + 2, *pos1, *(pos1 + 1), *(pos1 + 2), *(pos1 + 3), *(pos1 + 4), *(pos1 + 5));
	}
	for (int i = 0; i < triCount; ++i)
	{
		printf("Tri: %d, indices: %d, %d, %d\n", i, indexData[(i * 3)], indexData[(i * 3) + 1], indexData[(i * 3) + 2]);
	}
}

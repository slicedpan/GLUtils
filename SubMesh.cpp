#include "SubMesh.h"
#include "ObjLoader/objLoader.h"
#include "DebugDraw.h"
#include <GL/glew.h>
#include <cstdlib>
#include <svl\SVL.h>

Vec3 FromFloatV(float * ptr)
{
	return Vec3(ptr[0], ptr[1], ptr[2]);
}

void SubMesh::LoadCached(unsigned char* data)	//we want to copy the data so caller can free it afterwards
{

	int offset = 0;

	memcpy(&meshInfo, data, sizeof(MeshInfo));
	offset += sizeof(MeshInfo);	

	meshData = (float*)malloc(meshInfo.vertexCount * meshInfo.vertexSize);
	memcpy(meshData, data + offset, meshInfo.vertexCount * meshInfo.vertexSize);
	offset += meshInfo.vertexCount * meshInfo.vertexSize;

	switch (meshInfo.indexFormat)
	{
	case GL_UNSIGNED_INT:
		longIndexData = (unsigned int*)malloc(sizeof(unsigned int) * 3 * meshInfo.triCount);
		memcpy(longIndexData, data + offset, sizeof(unsigned int) * 3 * meshInfo.triCount);
		break;
	case GL_UNSIGNED_SHORT:
		indexData = (unsigned short*)malloc(sizeof(unsigned short) * 3 * meshInfo.triCount);
		memcpy(indexData, data + offset, sizeof(unsigned short) * 3 * meshInfo.triCount);
		break;
	case GL_UNSIGNED_BYTE:
		byteIndexData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * meshInfo.triCount);
		memcpy(byteIndexData, data + offset, sizeof(unsigned char) * 3 * meshInfo.triCount);
		break;
	}

	InitialiseVAO();
}

SubMeshBlock SubMesh::GenerateCache()
{
	
	SubMeshBlock smb;

	smb.size = 0;

	smb.size += sizeof(MeshInfo);

	smb.size += meshInfo.vertexSize * meshInfo.vertexCount;
	size_t indexSize;

	switch (meshInfo.indexFormat)
	{
	case GL_UNSIGNED_INT:
		indexSize = sizeof(unsigned int);
		
		break;
	case GL_UNSIGNED_SHORT:
		indexSize = sizeof(unsigned short);
		break;
	case GL_UNSIGNED_BYTE:
		indexSize = sizeof(unsigned char);
		break;
	}

	smb.size += indexSize * meshInfo.triCount * 3;

	smb.data = (unsigned char*)malloc(smb.size);

	int offset = 0;

	memcpy(smb.data, (void*)&meshInfo, sizeof(meshInfo));

	offset += sizeof(meshInfo);

	memcpy(smb.data + offset, meshData, meshInfo.vertexCount * meshInfo.vertexSize);

	offset += meshInfo.vertexCount * meshInfo.vertexSize;	

	switch (meshInfo.indexFormat)	//index data
	{
	case GL_UNSIGNED_INT:
		memcpy(smb.data, longIndexData, indexSize * meshInfo.triCount * 3);
		break;
	case GL_UNSIGNED_SHORT:
		memcpy(smb.data, indexData, indexSize * meshInfo.triCount * 3);
		break;
	case GL_UNSIGNED_BYTE:
		memcpy(smb.data, byteIndexData, indexSize * meshInfo.triCount * 3);
		break;
	}

	return smb;

}

void SubMesh::LoadObj(objLoader* obj, bool smoothNormals, bool generateNormals)
{

	memset(&meshInfo, 0, sizeof(MeshInfo));

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

			for (int k = 0; k < 3; ++k)
			{
				if (obj->vertexList[obj->faceList[i]->vertex_index[j]]->e[k] > (*max)[k])
					(*max)[k] = obj->vertexList[obj->faceList[i]->vertex_index[j]]->e[k];
				if (obj->vertexList[obj->faceList[i]->vertex_index[j]]->e[k] < (*min)[k])
					(*min)[k] = obj->vertexList[obj->faceList[i]->vertex_index[j]]->e[k];
			}

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

			switch (meshInfo.indexFormat)
			{
			case GL_UNSIGNED_INT:
				longIndexData[(i * 3) + j] = indexCount++;
				break;
			case GL_UNSIGNED_SHORT:
				indexData[(i * 3) + j] = indexCount++;
				break;
			case GL_UNSIGNED_BYTE:
				byteIndexData[(i * 3) + j] = indexCount++;
				break;
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
}

void SubMesh::InitialiseVAO()
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

	if (HasNormals())
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, meshInfo.vertexSize, (void*)(3 * sizeof(float)));
		if (HasTextureCoords())
		{
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, meshInfo.vertexSize, (char*)0 + 6 * sizeof(float));
		}
	}
	else if (HasTextureCoords())
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, meshInfo.vertexSize, (char*)0 + 3 * sizeof(float));
	}	

	glBindVertexArray(0);
	loaded = true;
}

SubMesh::SubMesh()
	: meshData(0),
	indexData(0),
	vaoID(0),
	bufID(0),
	indexBufID(0),
	loaded(false),
	obj(0),
	min(new Vec3(FLT_MAX, FLT_MAX, FLT_MAX)),
	max(new Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX)),
	Min(*min),
	Max(*max)
{

}

void SubMesh::CleanUp()
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
	if (obj)
	{
		delete obj;
		obj = 0;
	}
	loaded = false;
}

SubMesh::~SubMesh(void)
{
	
}

void SubMesh::Draw()
{
	if (!loaded)
		return;
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, meshInfo.triCount * 3, meshInfo.indexFormat, (void*)0);
	glBindVertexArray(0);
}

Triangle SubMesh::GetTriangle(int triIndex)
{
	Triangle t;
	switch (meshInfo.indexFormat)
	{
	case GL_UNSIGNED_INT:
		memcpy(t.index, longIndexData + triIndex * 3, sizeof(unsigned int) * 3); 
		break;
	case GL_UNSIGNED_SHORT:
		t.index[0] = *(indexData + triIndex * 3);
		t.index[1] = *(indexData + triIndex * 3 + 1);
		t.index[2] = *(indexData + triIndex * 3 + 2);
		break;
	case GL_UNSIGNED_BYTE:
		t.index[0] = *(byteIndexData + triIndex * 3);
		t.index[1] = *(byteIndexData + triIndex * 3 + 1);
		t.index[2] = *(byteIndexData + triIndex * 3 + 2);
		break;
	}
	return t;
}

void SubMesh::DrawImmediate()
{
	if (!loaded)
		return;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < meshInfo.triCount; ++i)
	{
		Triangle t = GetTriangle(i);
		if (HasTextureCoords())
		{
			for (int j = 0; j < 3; ++j)
			{
				VertexPositionTexcoord* vpt = GetVertexData<VertexPositionTexcoord>(t.index[j]);
				glVertex3fv(vpt->position);
				glTexCoord2fv(vpt->texCoord);
			}
		}
		else
		{	
			for (int j = 0; j < 3; ++j)
			{
				VertexPositionNormalTexcoord* vpnt = GetVertexData<VertexPositionNormalTexcoord>(t.index[j]);
				glVertex3fv(vpnt->position);
				if (HasNormals())
					glNormal3fv(vpnt->normal);
				if (HasTextureCoords())
					glTexCoord2fv(vpnt->texCoord);
			}
		}
	}
	glEnd();
}

void SubMesh::Print()
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

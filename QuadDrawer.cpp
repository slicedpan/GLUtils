#include "QuadDrawer.h"
#include <cstdlib>
#include <GL\glew.h>
#include <svl\SVL.h>

bool QuadDrawer::initialised = false;
QuadDrawer* QuadDrawer::instance = 0;

void QuadDrawer::DrawQuad(Vec2& max, Vec2& min)
{
	if (!instance)
		Initialise();
	instance->vertexData[0].position[0] = min[0];
	instance->vertexData[0].position[1] = min[1];

	instance->vertexData[1].position[0] = max[0];
	instance->vertexData[1].position[1] = min[1];

	instance->vertexData[2].position[0] = max[0];
	instance->vertexData[2].position[1] = max[1];

	instance->vertexData[3].position[0] = min[0];
	instance->vertexData[3].position[1] = max[0];

	glBindBuffer(GL_ARRAY_BUFFER, instance->vboID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->iboID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

}

void QuadDrawer::Initialise()
{
	instance = new QuadDrawer();
	glGenBuffers(1, &instance->vboID);
	glGenBuffers(1, &instance->iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->iboID);
	instance->indices = (unsigned short*)malloc(sizeof(unsigned short) * 6);
	instance->indices[0] = 0;
	instance->indices[1] = 1;
	instance->indices[2] = 2;
	instance->indices[3] = 0;
	instance->indices[4] = 2;
	instance->indices[5] = 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 6, instance->indices, GL_STATIC_DRAW);

	instance->vertexData = (vertex*)malloc(sizeof(vertex) * 4);

	instance->vertexData[0].texCoord[0] = 0.0;
	instance->vertexData[0].texCoord[1] = 0.0;

	instance->vertexData[1].texCoord[0] = 1.0;
	instance->vertexData[1].texCoord[1] = 0.0;

	instance->vertexData[2].texCoord[0] = 1.0;
	instance->vertexData[2].texCoord[1] = 1.0;

	instance->vertexData[3].texCoord[0] = 0.0;
	instance->vertexData[3].texCoord[1] = 1.0;

	glBindBuffer(GL_ARRAY_BUFFER, instance->vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 4, instance->vertexData, GL_DYNAMIC_DRAW);
}

void QuadDrawer::CleanUp()
{
	delete instance;
}

QuadDrawer::~QuadDrawer()
{
	free(indices);
	free(vertexData);
}

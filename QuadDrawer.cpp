#include "QuadDrawer.h"
#include "ShaderManager.h"
#include <cstdlib>
#include <GL\glew.h>
#include <svl\SVL.h>

bool QuadDrawer::initialised = false;
QuadDrawer* QuadDrawer::instance = 0;

void QuadDrawer::DrawQuad(Vec2& min, Vec2& max)
{
	if (!instance)
		Initialise();
	DrawQuad(min, max, *(instance->zero));
}

void QuadDrawer::DrawQuads(int amount)
{
	if (!instance)
		Initialise();
	glBindVertexArray(instance->vaoID);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0, amount);
	glBindVertexArray(0);
}

void QuadDrawer::DrawQuad(Vec2& min, Vec2& max, Vec2& pixSize)
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
	instance->vertexData[3].position[1] = max[1];

	if (ShaderManager::GetSingletonPtr()->GetCurrent())
		ShaderManager::GetSingletonPtr()->GetCurrent()->Uniforms["pixSize"].SetValue(pixSize);	

	glBindBuffer(GL_ARRAY_BUFFER, instance->vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 4, instance->vertexData, GL_DYNAMIC_DRAW);

	glBindVertexArray(instance->vaoID);	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
	glBindVertexArray(0);
}

void QuadDrawer::Initialise()
{
	instance = new QuadDrawer();
	glGenBuffers(1, &instance->vboID);
	glGenBuffers(1, &instance->iboID);
	glGenVertexArrays(1, &instance->vaoID);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->iboID);
	instance->indices = (unsigned char*)malloc(sizeof(unsigned char) * 6);
	instance->indices[0] = 0;
	instance->indices[1] = 1;
	instance->indices[2] = 2;
	instance->indices[3] = 0;
	instance->indices[4] = 2;
	instance->indices[5] = 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char) * 6, instance->indices, GL_STATIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(instance->vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, instance->vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->iboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (char*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (char*)0 + (2 * sizeof(float)));

	glBindVertexArray(0);

	instance->zero = new Vec2(0.0, 0.0);

	initialised = true;

}

void QuadDrawer::CleanUp()
{
	delete instance;
}

QuadDrawer::~QuadDrawer()
{
	free(indices);
	free(vertexData);
	delete zero;
}

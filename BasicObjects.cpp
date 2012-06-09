#include "BasicObjects.h"
#include <GL\glew.h>
#include <svl\SVL.h>
#include <smart_ptr\scoped_ptr.hpp>
#include "Shader.h"

CoordFrame::CoordFrame()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 3;
	indexData[4] = 4;
	indexData[5] = 5;

	Vec3 origin(0.0f, 0.0f, 0.0f);
	Vec4 black(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 6; ++i)
	{
		memcpy(&(vertices[i].position), origin.Ref(), sizeof(float) * 3);
		memcpy(&(vertices[i].colour), black.Ref(), sizeof(float) * 4);
	}

	vertices[0].colour[0] = 1.0f;
	vertices[1].colour[0] = 1.0f;
	vertices[1].position[0] = 1.0f;

	vertices[2].colour[1] = 1.0f;
	vertices[3].colour[1] = 1.0f;
	vertices[3].position[1] = 1.0f;

	vertices[4].colour[2] = 1.0f;
	vertices[5].colour[2] = 1.0f;
	vertices[5].position[2] = 1.0f;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColour), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColour), (void*) (sizeof(float) * 4));

	glBindVertexArray(0);

	shader = new Shader("DrawCoordFrame");
	shader->SetSourceFiles("Assets/Shaders/coord.vert", "Assets/Shaders/coord.frag");
	
	shader->Compile();
}

CoordFrame::~CoordFrame()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
}

void CoordFrame::Draw(Mat4& wvp)
{
	shader->Use();
	shader->Uniforms["wvp"].SetValue(wvp);
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, (void*)0);
}

boost::scoped_ptr<CoordFrame> _coordFrame;

void DrawCoordFrame(Mat4& wvp)
{
	if (!_coordFrame.get())
		_coordFrame.reset(new CoordFrame());
	_coordFrame->Draw(wvp);
}
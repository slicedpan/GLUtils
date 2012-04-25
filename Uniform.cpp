#include "Uniform.h"
#include <svl\SVL.h>
#include <GL\glew.h>

Uniform::Uniform(std::string name, unsigned int id)
	: name(name),
	id(id)
{
}

Uniform::~Uniform(void)
{
}

#pragma region SetValue overloads

void Uniform::SetValue(Mat4& matrix) const
{
	if (id >= 0)
		glUniformMatrix4fv(id, 1, false, matrix.Ref());
}
void Uniform::SetValue(Mat3& matrix) const
{
	if (id >= 0)
		glUniformMatrix3fv(id, 1, false, matrix.Ref());
}
void Uniform::SetValue(Vec2& vector) const
{
	if (id >= 0)
		glUniform2fv(id, 1, vector.Ref());
}
void Uniform::SetValue(Vec3& vector) const
{
	if(id >= 0)
		glUniform3fv(id, 1, vector.Ref());
}
void Uniform::SetValue(Vec4& vector) const
{
	if (id >= 0)
		glUniform4fv(id, 1, vector.Ref());
}

void Uniform::SetValue(int intVal) const
{
	if (id >= 0)
		glUniform1i(id, intVal);
}

void Uniform::SetValue(float floatVal) const
{
	if (id >= 0)
		glUniform1f(id, floatVal);
}

#pragma endregion

#pragma region SetArrayValue overloads

void Uniform::SetArrayValue(int* intValues, int count) const
{
	if (id >= 0)
		glUniform1iv(id, count, intValues);
}

void Uniform::SetArrayValue(float* floatValues, int count) const
{
	if (id >= 0)
		glUniform1fv(id, count, floatValues);
}

#pragma endregion



#ifndef BASICOBJECTS_H_
#define BASICOBJECTS_H_

class Mat4;
class Shader;

#include <GL\glew.h>

struct VertexPositionColour
{
	float position[4]; //padding
	float colour[4];
};

class CoordFrame
{
public:
	CoordFrame();
	~CoordFrame();
	void Draw(Mat4& wvp);
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	Shader* shader;
	VertexPositionColour vertices[6];
	char indexData[6];
};

class ovec2
{
private:
	class _swizzle
	{
		ovec2& ref;
	public:
		ovec2& operator= (const ovec2& other)
		{
			ref.x = other.y;
			ref.y = other.x;
			return ref;
		}
		ovec2& GetRef() const
		{
			return ref;
		}
		_swizzle(ovec2& self) : ref(self) {}
	}; 
public:
	float x, y;	
	ovec2& operator= (const _swizzle other)
	{
		x = other.GetRef().y;
		y = other.GetRef().x;
		return *this;
	}	
	_swizzle yx;
	ovec2() : yx(*this) {}
};

void DrawCoordFrame(Mat4& wvp);

#endif

#ifndef BASICOBJECTS_H_
#define BASICOBJECTS_H_

class Mat4;
class Shader;

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

void DrawCoordFrame(Mat4& wvp);

#endif

#pragma once

#include <string>

class Mat4;
class Mat3;
class Vec2;
class Vec3;
class Vec4;

class Uniform
{
public:
	Uniform(std::string name, unsigned int id);
	Uniform() : name(""), id(-1) {}
	~Uniform(void);
	void SetValue(Mat4& matrix) const;
	void SetValue(Mat3& matrix) const;
	void SetValue(Vec2& vector) const;
	void SetValue(Vec3& vector) const;
	void SetValue(Vec4& vector) const;
	void SetValue(int intVal) const;
	void SetValue(float floatVal) const;
	void SetArrayValue(int* intValues, int count) const;
	void SetArrayValue(float* floatValues, int count) const;	
	std::string GetName() const { return name; }
	unsigned int GetId() const { return id; }
private:
	std::string name;
	int id;
};


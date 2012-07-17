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
	void SetValue(const Mat4& matrix) const;
	void SetValue(const Mat3& matrix) const;
	void SetValue(const Vec2& vector) const;
	void SetValue(const Vec3& vector) const;
	void SetValue(const Vec4& vector) const;
	void SetValue(int i1, int i2) const;
	void SetValue(int i1, int i2, int i3) const;
	void SetValue(int i1, int i2, int i3, int i4) const;
	void SetValue(int intVal) const;
	void SetValue(float floatVal) const;
	void SetArrayValue(const int* intValues, int count) const;
	void SetArrayValue(const float* floatValues, int count) const;	
	std::string GetName() const { return name; }
	unsigned int GetId() const { return id; }
private:
	std::string name;
	int id;
};


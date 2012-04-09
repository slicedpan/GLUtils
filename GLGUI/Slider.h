#pragma once
#include "guielement.h"
#include "Primitives.h"
#include "glFunctions.h"
#include <string>

template<typename T>
class Slider : public GUIElement
{
public:
	Slider(T min, T max, T* value, std::string label) : min(min), max(max), value(value), label(label) 
	{
		width = label.length() * 9 + 150;
		height = 30;
	}
	~Slider(void);
	void Draw(Vec2& position)
	{
		DrawBGQuad(colour, position, width, height, 3);
	}
private:
	T min;
	T max;
	T* value;
	std::string label;
};


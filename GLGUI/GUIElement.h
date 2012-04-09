#pragma once

#include <svl\SVL.h>

class GUIElement
{
public:
	GUIElement();
	~GUIElement(void);
	virtual void Draw(Vec2& position) = 0;
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	void SetColour(Vec4& colour) { this->colour = colour; }
	void SetTextColour(Vec4& colour) { this->textColour = colour; }
protected:
	int width;
	int height;
	Vec4 colour;
	Vec4 textColour;
};


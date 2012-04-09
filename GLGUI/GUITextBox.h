#pragma once

#include "GUIElement.h"
#include <string>

class GUITextBox : public GUIElement
{
public:
	GUITextBox(std::string label);
	~GUITextBox(void);
	void Draw(Vec2& position);
	std::string label;
};


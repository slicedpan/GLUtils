#include "GUITextBox.h"
#include "Primitives.h"

GUITextBox::GUITextBox(std::string label) : label(label)
{
	width = label.length() * 7 + 8;
	height = 21;
}

GUITextBox::~GUITextBox(void)
{
}

void GUITextBox::Draw(Vec2& position)
{
	DrawBGQuad(colour * 0.5f, position, width, height, 1, true);
	PrintText(position + Vec2(3, 3), label.c_str(), textColour);
}

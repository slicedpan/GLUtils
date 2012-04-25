#include "GUITray.h"
#include "GUIElement.h"
#include "Primitives.h"
#include "GUITextBox.h"
#include <GL\glew.h>

GUITray::GUITray(int xPos, int yPos, unsigned int growDirection)
{
	width = 100;
	height = 0;
	borderwidth = 5;
	position[0] = xPos;
	position[1] = yPos;
	colour[0] = 1.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
	colour[3] = 0.8f;
	textColour = Vec4(0, 0, 0, 1);
}

GUITray::~GUITray(void)
{
}

void GUITray::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Background quad
	
	DrawBGQuad(colour, position, width + (borderwidth * 2) + 6, height + (borderwidth * 2) + 3 + (elements.size() * 3), borderwidth);

	Vec2 curPos = position + Vec2(borderwidth + 3, borderwidth + 3);

	for (int i = 0; i < elements.size(); ++i)
	{
		elements[i]->Draw(curPos);
		curPos += Vec2(0, elements[i]->GetHeight() + 3);
	}
}

void GUITray::AddElement(GUIElement* element)
{
	elements.push_back(element);
	if (element->GetWidth() > width)
		width = element->GetWidth();
	height += element->GetHeight();
	element->SetColour(colour);
	element->SetTextColour(textColour);
}

void GUITray::AddTextBox(std::string text)
{
	GUITextBox* textBox = new GUITextBox(text);
	textBox->SetColour(colour);	
	textBox->SetTextColour(textColour);
	AddElement(textBox);
}

bool GUITray::MouseClick(int x, int y, int button)
{
	return false;
}

void GUITray::MouseRelease(int x, int y, int button)
{

}

void GUITray::MouseMove(int x, int y)
{

}

void GUITray::SetColour(Vec4& colour)
{
	this->colour = colour;
	for (int i = 0; i < elements.size(); ++i)
	{
		elements[i]->SetColour(colour);
	}
}

void GUITray::SetTextColour(Vec4& textColour)
{
	this->textColour = textColour;
	for (int i = 0; i < elements.size(); ++i)
	{
		elements[i]->SetTextColour(textColour);
	}
}

#pragma once

#include <vector>
#include <svl/svl.h>

class GUIElement;

class GUITray
{
public:
	GUITray(int xPos, int yPos, unsigned int growDirection);
	~GUITray(void);
	void Draw();
	bool MouseClick(int x, int y, int button);
	void MouseRelease(int x, int y, int button);
	void MouseMove(int x, int y);
	void AddTextBox(std::string text);
	void AddElement(GUIElement* element);
	void SetColour(Vec4& colour);
	void SetTextColour(Vec4& textColour);
private:
	std::vector<GUIElement*> elements;
	Vec4 colour;
	Vec4 textColour;
	Vec2 position;
	int height;
	int width;
	int borderwidth;
	void DrawLBorder(bool repeat);
	void DrawTBorder(bool repeat);
	void DrawRBorder(bool repeat);
	void DrawBBorder(bool repeat);
};


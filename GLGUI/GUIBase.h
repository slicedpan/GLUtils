#pragma once

#include <vector>

class GUITray;

class GUIBase
{
public:
	GUIBase(void);
	~GUIBase(void);
	void Draw();
	void Update();
	void MouseMove(int x, int y);
	void MouseClick(int x, int y, int button);
	void MouseRelease(int x, int y, int button);
	GUITray* CreateTray(int xPos, int yPos, unsigned int growDirection);
private:
	std::vector<GUITray*> trays;
	GUITray* focusTray;
};




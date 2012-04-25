#include "GUIBase.h"
#include "GUITray.h"
#include <GL\glew.h>

GUIBase::GUIBase(void)
{
}

GUIBase::~GUIBase(void)
{
}

void GUIBase::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < trays.size(); ++i)
	{
		trays[i]->Draw();
	}
}

void GUIBase::MouseClick(int x, int y, int button)
{
	for (int i = 0; i < trays.size(); ++i)
	{
		if (trays[i]->MouseClick(x, y, button))
		{
			focusTray = trays[i];
			break;
		}
	}
}

void GUIBase::MouseRelease(int x, int y, int button)
{
	if (focusTray)
		focusTray->MouseRelease(x, y, button);
	focusTray = 0;
}

void GUIBase::MouseMove(int x, int y)
{
	if (focusTray)
		focusTray->MouseMove(x, y);
}

void GUIBase::Update()
{

}

GUITray* GUIBase::CreateTray(int xPos, int yPos, unsigned int growDirection)
{
	trays.push_back(new GUITray(xPos, yPos, growDirection));
	return trays[trays.size() - 1];
}




#ifndef GLFWKEYLISTENER_H
#define GLFWKEYLISTENER_H

#include "MiscUtils.h"

class GLFWKeyListener
{
public:
	GLFWKeyListener(void);
	~GLFWKeyListener(void);
	void _KeyboardEvent(int code, int state);
	virtual void KeyPressed(int code) {}
	virtual void KeyReleased(int code) {}
	ListWrapper<BitArray, bool> KeyState;
protected:
	void KeyInit();
private:
	BitArray keyState;
};

#endif


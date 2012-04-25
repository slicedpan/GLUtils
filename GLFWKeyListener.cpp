#include "GLFWKeyListener.h"
#include <vector>
#include <GL\glfw.h>

static bool initialised = false;
static std::vector<GLFWKeyListener*> listeners;

void KeyboardCallback(int code, int state)
{
	for (int i = 0; i < listeners.size(); ++i)
	{
		listeners[i]->_KeyboardEvent(code, state);
	}
}

GLFWKeyListener::GLFWKeyListener(void)
	: keyState(384),
	KeyState(keyState)
{
	if (!initialised)
		glfwSetKeyCallback(KeyboardCallback);
	listeners.push_back(this);
}

GLFWKeyListener::~GLFWKeyListener(void)
{
	for (int i = 0; i < listeners.size(); ++i)
	{
		if (listeners[i] == this)
		{
			listeners.erase(listeners.begin() + i);
			break;
		}
	}
}

void GLFWKeyListener::_KeyboardEvent(int code, int state)
{
	keyState.Set(code, state);
	if (state == GLFW_PRESS)
		KeyPressed(code);
	else
		KeyReleased(code);
}


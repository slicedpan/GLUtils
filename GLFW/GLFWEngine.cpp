#include "GLFWEngine.h"
#include <GL\glfw.h>

GLFWEngine::GLFWEngine(void) : BasicEngine(new GLFWTimerProvider())
{
}

GLFWEngine::~GLFWEngine(void)
{
}

void GLFWEngine::EndFrame()
{
	glfwSwapBuffers();
	if (!glfwGetWindowParam(GLFW_OPENED))
		Exit();
}

double GLFWTimerProvider::GetTimeElapsed()
{
	return glfwGetTime();
}

void GLFWTimerProvider::Sleep(double amount)
{
	glfwSleep(amount);
}


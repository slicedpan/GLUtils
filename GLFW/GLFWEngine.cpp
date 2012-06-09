#include "GLFWEngine.h"
#include "../WindowSettings.h"
#include <GL\glew.h>
#include <GL\glfw.h>

GLFWEngine::GLFWEngine(void) : BasicEngine(new GLFWTimerProvider()),
	currentSettings(new WindowSettings()),
	Window(*currentSettings)
{
	WindowSettings w;

	if (!InitWindow(w))
		printf("Could not open window!");

	KeyInit();

}

GLFWEngine::GLFWEngine(WindowSettings& w) : BasicEngine(new GLFWTimerProvider()),
	currentSettings(new WindowSettings()),
	Window(*currentSettings)
{
	if (!InitWindow(w))
		printf("Could not open window");

	KeyInit();

}

int GLFWEngine::InitWindow(WindowSettings& w)
{

	if (!glfwInit())
		throw;

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, w.OpenGLVersionMajor);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, w.OpenGLVersionMinor);

	int windowType = w.Fullscreen ? GLFW_FULLSCREEN: GLFW_WINDOW;

	int err = glfwOpenWindow(w.Width, w.Height, w.RedBits, w.GreenBits, w.BlueBits, w.AlphaBits, w.DepthBits, w.StencilBits, windowType);

	glfwSetWindowTitle(w.Title.c_str());

	*currentSettings = w;

	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK)
	{
		printf("Glew Error: %s\n", glewGetErrorString(glewErr));
		return 1;
	}

	return err;
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



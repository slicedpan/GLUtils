#ifndef GLFWENGINE_H
#define GLFWENGINE_H

#include "../BasicEngine.h"
#include "../GLFWKeyListener.h"

struct WindowSettings;

class GLFWTimerProvider : public ITimerProvider
{
public:
	double GetTimeElapsed();
	void Sleep(double amount);
};

class GLFWEngine :
	public BasicEngine, public GLFWKeyListener
{
private:
	int InitWindow(WindowSettings& windowSettings);
	WindowSettings* currentSettings;
public:
	GLFWEngine(void);
	GLFWEngine(WindowSettings& windowSettings);
	virtual ~GLFWEngine(void);	
	void EndFrame();
	WindowSettings& Window;

};

#endif

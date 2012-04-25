#ifndef GLFWENGINE_H
#define GLFWENGINE_H

#include "../BasicEngine.h"
#include "../GLFWKeyListener.h"

class GLFWTimerProvider : public ITimerProvider
{
public:
	double GetTimeElapsed();
	void Sleep(double amount);
};

class GLFWEngine :
	public BasicEngine, public GLFWKeyListener
{
public:
	GLFWEngine(void);
	~GLFWEngine(void);	
	void EndFrame();
};

#endif

#ifndef GLFWENGINE_H
#define GLFWENGINE_H

#include "../BasicEngine.h"

class GLFWTimerProvider : public ITimerProvider
{
public:
	double GetTimeElapsed();
	void Sleep(double amount);
};

class GLFWEngine :
	public BasicEngine
{
public:
	GLFWEngine(void);
	~GLFWEngine(void);	
	void EndFrame();
};

#endif

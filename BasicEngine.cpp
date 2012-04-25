#include "BasicEngine.h"

BasicEngine::BasicEngine(ITimerProvider* timerProvider)
	: timerProvider(timerProvider),
	MaxFPS(60),
	LimitFPS(true),
	running(true),
	CurrentFPS(currentFPS),
	frameCounter(0),
	timeElapsed(0.0),
	timeCount(0.0),
	frameBegin(0.0),
	lastTime(0.0)
{
}

BasicEngine::~BasicEngine(void)
{
}

void BasicEngine::Run()
{
	Setup();
	while (running)
	{
		frameBegin = timerProvider->GetTimeElapsed();
		timeElapsed = frameBegin - lastTime;

		++frameCounter;		
		timeCount += timeElapsed;		
		if (timeCount > 1.0)
		{
			currentFPS = frameCounter;
			frameCounter = 0;
			timeCount = 0.0;
		}
		t.dTimeElapsed = frameBegin;
		t.dTimeSinceLastFrame = timeElapsed;
		t.fTimeElapsed = (float)frameBegin;
		t.fTimeSinceLastFrame = (float)timeElapsed;
		Update(t);		
		Display();
		if (LimitFPS)
		{
			timerProvider->Sleep(0.016 - timerProvider->GetTimeElapsed() + frameBegin);
		}
		EndFrame();
		lastTime = frameBegin;
	}
}

void BasicEngine::Exit()
{
	running = false;
}



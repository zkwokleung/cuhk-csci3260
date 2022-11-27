#include "Time.h"

float Time::s_lastFrameTime = 0;
float Time::s_deltaTime = 0;

float Time::GetTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

float Time::GetDeltaTime()
{
	return Time::s_deltaTime;
}

void Time::OnFrameStart(void)
{
	float currentTime = GetTime() * 0.001;

	s_deltaTime = currentTime - s_lastFrameTime;

	s_lastFrameTime = currentTime;
}

void Time::OnFrameEnd(void)
{

}


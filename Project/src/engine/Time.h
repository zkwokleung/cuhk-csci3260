#pragma once
#include <glew/glew.h>
#include <freeglut/glut.h>
#include "../api/debugModule.h"

class Time
{
public:
	// Get the time elasped since the game started
	static float GetTime();
	// The time elasped between the last frame and this frame
	static float GetDeltaTime();

	static void OnFrameStart(void);
	static void OnFrameEnd(void);
private:
	static float s_lastFrameTime;
	static float s_deltaTime;
};
#pragma once
#include <stdlib.h>
#include <ctime>

class Random
{
public:
	static void Init(void);
	static float Range(float min, float max);
	static int Range(int min, int max);
};
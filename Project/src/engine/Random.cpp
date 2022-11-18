#include "Random.h"

void Random::Init(void)
{
	srand(static_cast <unsigned> (time(0)));
}

float Random::Range(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));;
}

int Random::Range(int min, int max)
{
	return rand() % max + min;
}

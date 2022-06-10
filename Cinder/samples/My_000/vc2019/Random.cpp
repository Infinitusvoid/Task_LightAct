#include "Random.h"

Random& Random::Get()
{
	static Random instance;
	return instance;
}

Random::Random() //Private constructor
{
	rand = cinder::Rand();
}

float Random::rnd_01()
{
	return Get().inter_rnd_01();
}

float Random::nextFloat(float a, float b)
{
	return Get().inter_nextFloat(a, b);
}

uint32_t Random::nextUint()
{
	return Get().inter_nextUint();
}

uint32_t Random::inter_nextUint()
{
	return rand.nextUint();
}

float Random::inter_nextFloat(float a, float b)
{
	return rand.nextFloat(a, b);
}

float Random::inter_rnd_01()
{
	return rand.nextFloat(0.0f, 1.0f);
}


#pragma once

#include "cinder/Rand.h"

class Random
{
public:
	Random(const Random&) = delete;
	static Random& Get();
	
	static float rnd_01();

	static float nextFloat(float a, float b);
	
	static uint32_t nextUint();
	

private:
	cinder::Rand rand;

	uint32_t inter_nextUint();
	
	float inter_nextFloat(float a, float b);
	
	float inter_rnd_01();
	
	Random();
};
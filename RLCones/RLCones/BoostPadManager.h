#pragma once
#include "BoostPad.h"
class BoostPadManager
{
public:
	BoostPadManager();
	BoostPad boostPads[34];
	const int boostPadsCount = 34;
	std::vector<BoostPad> customSpawns;
};



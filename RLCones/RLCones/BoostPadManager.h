#pragma once
#include "BoostPad.h"
#include "Cylinder2BoostPad.h"
#include "Course.h"

class BoostPadManager
{
private:
	
	

public:
	const int boostPadsCount = 34;
	BoostPadManager();
	std::vector<Cylinder2BoostPad> _customCylinder2Boostpads;
	Cylinder2BoostPad _standardCylinder2Boostpads[34];	
	Cylinder2BoostPad CreateCylinder2BoostPad(Vector spawnPos, bool isBigPad = false);
	void SpawnCustomCylinder2BoostPad(Vector spawnPos, bool isBigPad = false);
	void SpawnCustomCylinder2Gate(Vector spawnPos, float gateDistanceFromCar, bool isBigPad = false);
	void ClearCustomSpawns();
	void LoadCourse(Course course);
};



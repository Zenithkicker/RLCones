#include "pch.h"
#include "BoostPadManager.h"
#include "HelperFunctions.h"


BoostPadManager::BoostPadManager()
{
	_standardCylinder2Boostpads[0] = CreateCylinder2BoostPad(Vector(0.0, -4240.0, 90));
	_standardCylinder2Boostpads[1] = CreateCylinder2BoostPad(Vector(-1792.0, -4184.0, 90.0));
	_standardCylinder2Boostpads[2] = CreateCylinder2BoostPad(Vector(1792.0, -4184.0, 90.0));
	_standardCylinder2Boostpads[3] = CreateCylinder2BoostPad(Vector(-3072.0, -4096.0, 90.0),true);
	_standardCylinder2Boostpads[4] = CreateCylinder2BoostPad(Vector(3072.0, -4096.0, 90.0), true);
	_standardCylinder2Boostpads[5] = CreateCylinder2BoostPad(Vector(-940.0, -3308.0, 90.0));
	_standardCylinder2Boostpads[6] = CreateCylinder2BoostPad(Vector(940.0, -3308.0, 90.0));
	_standardCylinder2Boostpads[7] = CreateCylinder2BoostPad(Vector(0.0, -2816.0, 90.0));
	_standardCylinder2Boostpads[8] = CreateCylinder2BoostPad(Vector(-3584.0, -2484.0, 90.0));
	_standardCylinder2Boostpads[9] = CreateCylinder2BoostPad(Vector(3584.0, -2484.0, 90.0));
	_standardCylinder2Boostpads[10] = CreateCylinder2BoostPad(Vector(-1788.0, -2300.0, 90.0));
	_standardCylinder2Boostpads[11] = CreateCylinder2BoostPad(Vector(1788.0, -2300.0, 90.0));
	_standardCylinder2Boostpads[12] = CreateCylinder2BoostPad(Vector(-2048.0, -1036.0, 90.0));
	_standardCylinder2Boostpads[13] = CreateCylinder2BoostPad(Vector(0.0, -1024.0, 90.0));
	_standardCylinder2Boostpads[14] = CreateCylinder2BoostPad(Vector(2048.0, -1036.0, 90.0));
	_standardCylinder2Boostpads[15] = CreateCylinder2BoostPad(Vector(-3584.0, 0.0, 90.0), true);
	_standardCylinder2Boostpads[16] = CreateCylinder2BoostPad(Vector(-1024.0, 0.0, 90.0));
	_standardCylinder2Boostpads[17] = CreateCylinder2BoostPad(Vector(1024.0, 0.0, 90.0));
	_standardCylinder2Boostpads[18] = CreateCylinder2BoostPad(Vector(3584.0, 0.0, 90.0), true);
	_standardCylinder2Boostpads[19] = CreateCylinder2BoostPad(Vector(-2048.0, 1036.0, 90.0));
	_standardCylinder2Boostpads[20] = CreateCylinder2BoostPad(Vector(0.0, 1024.0, 90.0));
	_standardCylinder2Boostpads[21] = CreateCylinder2BoostPad(Vector(2048.0, 1036.0, 90.0));
	_standardCylinder2Boostpads[22] = CreateCylinder2BoostPad(Vector(-1788.0, 2300.0, 90.0));
	_standardCylinder2Boostpads[23] = CreateCylinder2BoostPad(Vector(1788.0, 2300.0, 90.0));
	_standardCylinder2Boostpads[24] = CreateCylinder2BoostPad(Vector(-3584.0, 2484.0, 90.0));
	_standardCylinder2Boostpads[25] = CreateCylinder2BoostPad(Vector(3584.0, 2484.0, 90.0));
	_standardCylinder2Boostpads[26] = CreateCylinder2BoostPad(Vector(0.0, 2816.0, 90.0));
	_standardCylinder2Boostpads[27] = CreateCylinder2BoostPad(Vector(-940.0, 3310.0, 90.0));
	_standardCylinder2Boostpads[28] = CreateCylinder2BoostPad(Vector(940.0, 3308.0, 90.0));
	_standardCylinder2Boostpads[29] = CreateCylinder2BoostPad(Vector(-3072.0, 4096.0, 90.0), true);
	_standardCylinder2Boostpads[30] = CreateCylinder2BoostPad(Vector(3072.0, 4096.0, 90.0), true);
	_standardCylinder2Boostpads[31] = CreateCylinder2BoostPad(Vector(-1792.0, 4184.0, 90.0));
	_standardCylinder2Boostpads[32] = CreateCylinder2BoostPad(Vector(1792.0, 4184.0, 90.0));
	_standardCylinder2Boostpads[33] = CreateCylinder2BoostPad(Vector(0.0, 4240.0, 90.0));
}

Cylinder2BoostPad BoostPadManager::CreateCylinder2BoostPad(Vector spawnPos, bool isBigPad)
{
	BoostPad pad = BoostPad(spawnPos, isBigPad);
	RT::Cylinder2 cy = RT::Cylinder2();	
	cy.location = pad.GetPosition();
	cy.height = pad.GetHeight();
	float padRadius = pad.GetRadius();
	cy.radiusBottom = padRadius;
	cy.radiusTop = padRadius * .5f;		
	return Cylinder2BoostPad(cy, pad);
}

void BoostPadManager::SpawnCustomCylinder2BoostPad(Vector spawnPos, bool isBigPad)
{
	BoostPad pad = BoostPad(spawnPos, isBigPad);
	RT::Cylinder2 cy = RT::Cylinder2();
	cy.location = pad.GetPosition();
	cy.height = pad.GetHeight();
	float padRadius = pad.GetRadius();
	cy.radiusBottom = padRadius;
	cy.radiusTop = padRadius * .5f;
	_customCylinder2Boostpads.push_back(Cylinder2BoostPad(cy, pad));
}

void BoostPadManager::SpawnCustomCylinder2Gate(CarWrapper cw, float gateDistanceFromCar, bool isBigPad)
{
	Vector carPos = cw.GetLocation();
	RBState carRb = cw.GetRBState();

	//get the right local vector of car
	Vector right = quatToRight(carRb.Quaternion);
	Vector left = -1 * right;

	//scale that vector by the gate distance
	Vector gateRightSpawn = right * gateDistanceFromCar;
	Vector gateLeftSpawn = left * gateDistanceFromCar;
	
	//create the spawn position of the gates by adding to the car position
	BoostPad padLeft = BoostPad(gateLeftSpawn + carPos, isBigPad);
	BoostPad padRight = BoostPad(gateRightSpawn + carPos, isBigPad);

	RT::Cylinder2 cyLeft = RT::Cylinder2();
	cyLeft.location = padLeft.GetPosition();
	cyLeft.height = padLeft.GetHeight();
	float padLeftRadius = padLeft.GetRadius();
	cyLeft.radiusBottom = padLeftRadius;
	cyLeft.radiusTop = padLeftRadius * .5f;
	_customCylinder2Boostpads.push_back(Cylinder2BoostPad(cyLeft, padLeft));

	RT::Cylinder2 cyRight = RT::Cylinder2();
	cyRight.location = padRight.GetPosition();
	cyRight.height = padRight.GetHeight();
	float padRightRadius = padRight.GetRadius();
	cyRight.radiusBottom = padRightRadius;
	cyRight.radiusTop = padRightRadius * .5f;
	_customCylinder2Boostpads.push_back(Cylinder2BoostPad(cyRight, padRight));
}

void BoostPadManager::ClearCustomSpawns() 
{
	_customCylinder2Boostpads.clear();
}

void BoostPadManager::LoadCourse(Course course)
{
	ClearCustomSpawns();

	std::vector<CourseItem> _courseItems = course.GetCourseItems();
	for (std::vector<CourseItem>::iterator it = _courseItems.begin(); it != _courseItems.end(); ++it)
	{
		_customCylinder2Boostpads.push_back(CreateCylinder2BoostPad(Vector(it->X, it->Y, it->Z), it->IsBig));
	}
}

json BoostPadManager::CreateCustomConesJson()
{
	json data;

	for (std::vector<Cylinder2BoostPad>::iterator it = _customCylinder2Boostpads.begin(); it != _customCylinder2Boostpads.end(); ++it)
	{
		Vector pos = it->_boostPad.GetPosition();
		bool isBig = it->_boostPad.GetIsBigPad();
		json item = {
			{"x",pos.X},
			{"y",pos.Y},
			{"z",pos.Z},
			{"isBig", isBig}
		};
		data.push_back(item);
	}
	json root;
	root["data"] = data;
	return root;
}
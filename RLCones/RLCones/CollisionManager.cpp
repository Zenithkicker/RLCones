#include "pch.h"
#include "CollisionManager.h"

bool CollisionManager::AreSpheresColliding(Vector sphere1Position, float sphere1Radius, Vector sphere2Position, float sphere2Radius) 
{
	Vector posDiff = sphere1Position - sphere2Position;
	float currentDistanceApart = posDiff.magnitude();
	float minNoCollideDistance = sphere1Radius + sphere2Radius;
	return currentDistanceApart <= minNoCollideDistance;
}
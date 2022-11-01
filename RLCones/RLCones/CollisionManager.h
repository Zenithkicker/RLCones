#pragma once
class CollisionManager
{
public:
	virtual bool AreSpheresColliding(Vector sphere1Position, float sphere1Radius, Vector sphere2Position, float sphere2Radius);
};


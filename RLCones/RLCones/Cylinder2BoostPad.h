#pragma once
#include "Objects/Cylinder2.h"
#include "Objects/Frustum.h"
#include "BoostPad.h"

class Cylinder2BoostPad
{
private:
	

public:
	Cylinder2BoostPad();
	Cylinder2BoostPad(RT::Cylinder2 cylinder2, BoostPad boostPad);
	void Draw(CanvasWrapper canvas, RT::Frustum &frustum);
	RT::Cylinder2 _cylinder2;
	BoostPad _boostPad;
};


#include "pch.h"
#include "Cylinder2BoostPad.h"

Cylinder2BoostPad::Cylinder2BoostPad() 
{

}

Cylinder2BoostPad::Cylinder2BoostPad(RT::Cylinder2 cylinder2, BoostPad boostPad)
{
	_cylinder2 = cylinder2;
	_boostPad = boostPad;
}

void Cylinder2BoostPad::Draw(CanvasWrapper canvas, RT::Frustum &frustum)
{
	_cylinder2.Draw(canvas, frustum);
}
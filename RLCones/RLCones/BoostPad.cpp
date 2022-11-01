#include "pch.h"
#include "BoostPad.h"

BoostPad::BoostPad() 
{
	position = Vector(0, 0, 0);
	IsBigPad = false;
}

BoostPad::BoostPad(Vector location) 
{
	position = location;	
	IsBigPad = false;
}

BoostPad::BoostPad(Vector location, bool bigPad)
{
	position = location;
	IsBigPad = bigPad;
}

Vector BoostPad::GetPosition() {
	return position;
}

float BoostPad::GetRadius() 
{
	if (IsBigPad)
		return 208;

	return 144;
}

float BoostPad::GetHeight() 
{
	if (IsBigPad)
		return 168;

	return 165;
}

void BoostPad::SetSpawnPosition(Vector desiredPosition)
{
	position = desiredPosition;
	position.Z = 70;
	if (IsBigPad) {
		position.Z = 73;
	}
}

void BoostPad::SetIsBigPad(bool isBig) 
{
	IsBigPad = isBig;
}

bool BoostPad::GetIsBigPad() 
{
	return IsBigPad;
}

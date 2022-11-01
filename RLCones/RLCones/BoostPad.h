#pragma once
class BoostPad
{
public:
	BoostPad();
	BoostPad(Vector location);
	BoostPad(Vector location, bool IsBigPad);	
	
	Vector GetPosition();
	float GetRadius();
	float GetHeight();
	void SetSpawnPosition(Vector desiredPosition);
	void SetIsBigPad(bool isBig);
	bool GetIsBigPad();

private:
	Vector position;
	bool IsBigPad;
};


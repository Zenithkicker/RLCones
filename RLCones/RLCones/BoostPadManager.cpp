#include "pch.h"
#include "BoostPadManager.h"


BoostPadManager::BoostPadManager()
{
	boostPads[0] = BoostPad(Vector(0.0, -4240.0, 70.0));
	boostPads[1] = BoostPad(Vector(-1792.0, -4184.0, 70.0));
	boostPads[2] = BoostPad(Vector(1792.0, -4184.0, 70.0));
	boostPads[3] = BoostPad(Vector(-3072.0, -4096.0, 73.0),true);
	boostPads[4] = BoostPad(Vector(3072.0, -4096.0, 73.0), true);
	boostPads[5] = BoostPad(Vector(-940.0, -3308.0, 70.0));
	boostPads[6] = BoostPad(Vector(940.0, -3308.0, 70.0));
	boostPads[7] = BoostPad(Vector(0.0, -2816.0, 70.0));
	boostPads[8] = BoostPad(Vector(-3584.0, -2484.0, 70.0));
	boostPads[9] = BoostPad(Vector(3584.0, -2484.0, 70.0));
	boostPads[10] = BoostPad(Vector(-1788.0, -2300.0, 70.0));
	boostPads[11] = BoostPad(Vector(1788.0, -2300.0, 70.0));
	boostPads[12] = BoostPad(Vector(-2048.0, -1036.0, 70.0));
	boostPads[13] = BoostPad(Vector(0.0, -1024.0, 70.0));
	boostPads[14] = BoostPad(Vector(2048.0, -1036.0, 70.0));
	boostPads[15] = BoostPad(Vector(-3584.0, 0.0, 73.0), true);
	boostPads[16] = BoostPad(Vector(-1024.0, 0.0, 70.0));
	boostPads[17] = BoostPad(Vector(1024.0, 0.0, 70.0));
	boostPads[18] = BoostPad(Vector(3584.0, 0.0, 73.0), true);
	boostPads[19] = BoostPad(Vector(-2048.0, 1036.0, 70.0));
	boostPads[20] = BoostPad(Vector(0.0, 1024.0, 70.0));
	boostPads[21] = BoostPad(Vector(2048.0, 1036.0, 70.0));
	boostPads[22] = BoostPad(Vector(-1788.0, 2300.0, 70.0));
	boostPads[23] = BoostPad(Vector(1788.0, 2300.0, 70.0));
	boostPads[24] = BoostPad(Vector(-3584.0, 2484.0, 70.0));
	boostPads[25] = BoostPad(Vector(3584.0, 2484.0, 70.0));
	boostPads[26] = BoostPad(Vector(0.0, 2816.0, 70.0));
	boostPads[27] = BoostPad(Vector(-940.0, 3310.0, 70.0));
	boostPads[28] = BoostPad(Vector(940.0, 3308.0, 70.0));
	boostPads[29] = BoostPad(Vector(-3072.0, 4096.0, 73.0), true);
	boostPads[30] = BoostPad(Vector(3072.0, 4096.0, 73.0), true);
	boostPads[31] = BoostPad(Vector(-1792.0, 4184.0, 70.0));
	boostPads[32] = BoostPad(Vector(1792.0, 4184.0, 70.0));
	boostPads[33] = BoostPad(Vector(0.0, 4240.0, 70.0));
}

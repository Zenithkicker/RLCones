#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Frustum;
    class Line;

	class Cylinder2
	{
	public:
		Vector location;
		Quat orientation;
		float radiusBottom;
		float radiusTop;
		float height;
		float lineThickness;

		// CONSTRUCTORS
		explicit Cylinder2();
		explicit Cylinder2(float radBottom, float radTop, float h);
		explicit Cylinder2(Vector loc, float radBottom, float radTop, float h);
		explicit Cylinder2(Vector loc, Quat rot, float radBottom, float radTop, float h);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum &frustum, int segments=16) const;		
	};
}
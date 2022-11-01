#include "../pch.h"
#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Cylinder2.h"
#include "Frustum.h"
#include "Line.h"
#include "Matrix3.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#include <vector>

RT::Cylinder2::Cylinder2()
	: location(Vector()), orientation(Quat()), radiusBottom(50), radiusTop(50), height(100), lineThickness(1) {}

RT::Cylinder2::Cylinder2(float radBottom, float radTop, float h)
	: location(Vector()), orientation(Quat()), radiusBottom(radBottom), radiusTop(radTop), height(h), lineThickness(1) {}

RT::Cylinder2::Cylinder2(Vector loc, float radBottom, float radTop, float h)
	: location(loc), orientation(Quat()), radiusBottom(radBottom), radiusTop(radTop), height(h), lineThickness(1) {}

RT::Cylinder2::Cylinder2(Vector loc, Quat rot, float radBottom, float radTop, float h)
	: location(loc), orientation(rot), radiusBottom(radBottom), radiusTop(radTop), height(h), lineThickness(1) {}

void RT::Cylinder2::Draw(CanvasWrapper canvas, Frustum &frustum, int segments) const
{
	//Simple frustum check. Not very clean but can be improved later
	if(!frustum.IsInFrustum(location, height * .5f))
	{
		return;
	}

	//Create circle
	std::vector<Vector> circlePointsBottom;
	std::vector<Vector> circlePointsTop;
	for(int j = 0; j != segments; ++j)
	{
		Quat rotAmount = AngleAxisRotation(2.f * CONST_PI_F * j / segments, Vector(0,0,1));
		circlePointsBottom.push_back(RotateVectorWithQuat(Vector(radiusBottom, 0, 0), rotAmount));
		circlePointsTop.push_back(RotateVectorWithQuat(Vector(radiusTop, 0, 0), rotAmount));
	}

	//Transform circles
	std::vector<Vector2F> bottomPoints;
	std::vector<Vector2F> topPoints;
	for(size_t i = 0; i != circlePointsBottom.size(); ++i)
	{
		Vector bottom = circlePointsBottom[i];
		Vector top = circlePointsTop[i];

		//Push circles to height of cylinder
		bottom.Z -= height * .5f;
		top.Z += height * .5f;

		//Rotate points to cylinder orientation
		bottom = RotateVectorWithQuat(bottom, orientation);
		top = RotateVectorWithQuat(top, orientation);

		//Translate to final location
		bottomPoints.push_back(canvas.ProjectF(bottom + location));
		topPoints.push_back(canvas.ProjectF(top + location));
	}

	//Draw lines
	for(size_t i = 0; i != bottomPoints.size(); ++i)
	{
		if(lineThickness == 1)
		{
			canvas.DrawLine(bottomPoints[i], topPoints[i]);
			if(i == bottomPoints.size() - 1)
			{
				//Draw from last to 0
				canvas.DrawLine(bottomPoints[i], bottomPoints[0]);
				canvas.DrawLine(topPoints[i], topPoints[0]);
			}
			else
			{
				canvas.DrawLine(bottomPoints[i], bottomPoints[i + 1]);
				canvas.DrawLine(topPoints[i], topPoints[i + 1]);
			}
		}
		else
		{
			canvas.DrawLine(bottomPoints[i], topPoints[i], lineThickness);
			if(i == bottomPoints.size() - 1)
			{
				//Draw from last to 0
				canvas.DrawLine(bottomPoints[i], bottomPoints[0], lineThickness);
				canvas.DrawLine(topPoints[i], topPoints[0], lineThickness);
			}
			else
			{
				canvas.DrawLine(bottomPoints[i], bottomPoints[i + 1], lineThickness);
				canvas.DrawLine(topPoints[i], topPoints[i + 1], lineThickness);
			}
		}
	}
}

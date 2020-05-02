#include "Triangle.h"
#include <cmath>
#include "Utils/Utils.h"


Triangle::Triangle(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2)
{
	mPoints.push_back(p0);
	mPoints.push_back(p1);
	mPoints.push_back(p2);
}

Triangle::Triangle(const Vec2D& centerPoint, unsigned int width, unsigned int height)
{
	
	Vec2D p0 = Vec2D(centerPoint.GetX(), centerPoint.GetY() - ((float)height * 2 / 3));
	Vec2D p1 = Vec2D(centerPoint.GetX() - (float)width / 3, centerPoint.GetY() + (float)height / 3);
	Vec2D p2 = Vec2D(centerPoint.GetX() + (float)width / 3, centerPoint.GetY() + (float)height / 3);

	mPoints.push_back(p0);
	mPoints.push_back(p1);
	mPoints.push_back(p2);
}

Triangle::Triangle():Triangle(Vec2D::Zero, Vec2D::Zero, Vec2D::Zero)
{}

Vec2D Triangle::GetCenterPoint() const
{
	return Vec2D((mPoints[0].GetX() + mPoints[1].GetX() + mPoints[2].GetX())/3, 
		(mPoints[0].GetY() + mPoints[1].GetY() + mPoints[2].GetY())/3);
}

void Triangle::MoveTo(const Vec2D& position)
{
	Vec2D deltaOffset = position - GetCenterPoint();
	MoveBy(deltaOffset);
}

float Triangle::Area() const
{
	return Area(GetP0(), GetP1(), GetP2());
}

void Triangle::Rotate(float angle)
{
	Vec2D center = GetCenterPoint();

	for (auto& point : mPoints)
	{
		point.Rotate(angle, center);
	}
}

bool Triangle::ContainsPoint(const Vec2D& p) const
{
	float thisArea = Area();

	float a1 = Area(p, GetP1(), GetP2());
	float a2 = Area(GetP0(), p, GetP2());
	float a3 = Area(GetP0(), GetP1(), p);

	return IsEqual(thisArea, a1 + a2 + a3);
}

float Triangle::Area(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2) const
// Formula followed explained in https://www.mathopenref.com/coordtrianglearea.html
{
	return fabsf((p0.GetX() * (p1.GetY() - p2.GetY()) + p1.GetX() * (p2.GetY() - p0.GetY()) + p2.GetX() * (p0.GetY() - p1.GetY())) / 2.0f);
}

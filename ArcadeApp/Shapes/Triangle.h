#pragma once

#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle();
	Triangle(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2);
	Triangle(const Vec2D& centerPoint, unsigned int width, unsigned int height);

	inline void SetP0(const Vec2D& p0) { mPoints[0] = p0; }
	inline void SetP1(const Vec2D& p1) { mPoints[1] = p1; }
	inline void SetP2(const Vec2D& p2) { mPoints[2] = p2; }

	inline Vec2D GetP0() const { return mPoints[0]; }
	inline Vec2D GetP1() const { return mPoints[1]; }
	inline Vec2D GetP2() const { return mPoints[2]; }


	virtual Vec2D GetCenterPoint() const override;
	virtual void MoveTo(const Vec2D& position) override;

	float Area() const;
	void Rotate(float angle);

	bool ContainsPoint(const Vec2D& p) const;

private:
	float Area(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2) const;

};
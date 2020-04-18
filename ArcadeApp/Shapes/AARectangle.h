#pragma once
#include "Shape.h"


class AARectangle : public Shape
// Axis aligned rectangle (not a shape with four points)
{
public:
	AARectangle();
	AARectangle(const Vec2D& topLeft, unsigned int width, unsigned int height);
	AARectangle(const Vec2D& topLeft, const Vec2D& bottomRight);
	
	inline void SetTopLeftPoint(const Vec2D& topLeft) { mPoints[0] = topLeft; }
	inline void SetBottomRight(const Vec2D& bottomRight) { mPoints[1] = bottomRight; }

	Vec2D GetTopLeft() const { return mPoints[0]; }
	Vec2D GetBottomRight() const { return mPoints[1]; }

	float GetWidth() const;
	float GetHeight() const;

	virtual void MoveTo(const Vec2D& position) override;
	virtual Vec2D GetCenterPoint() const override;

	bool Intersects(const AARectangle& otherRect) const;
	bool ContainsPoint(const Vec2D& p) const;

	static AARectangle Inset(const AARectangle& rect, Vec2D& insets);

	void PrintCoordinates() const;

	virtual std::vector<Vec2D> GetPoints() const override;
	
};
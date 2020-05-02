#pragma once
#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"

class Screen;

class Misile
{
public:
	void Init(const Vec2D& direction, const Vec2D& position);
	void Update(uint32_t dt, const AARectangle& boundary);
	void Draw(Screen& screen);

	inline Vec2D GetMisileLocation() const { return m_Boundary.GetCenterPoint(); }

private:
	Triangle m_Boundary;
	Vec2D m_Direction;
};
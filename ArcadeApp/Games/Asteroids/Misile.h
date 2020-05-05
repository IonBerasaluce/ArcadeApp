#pragma once
#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"

class Screen;

class Misile
{
public:

	Misile();
	void Init(const Vec2D& direction, const Vec2D& position);
	void Update(uint32_t dt, const AARectangle& boundary);
	void Draw(Screen& screen);
	void MoveTo(Vec2D position);

	void Hit();

	inline Vec2D GetPosition() const { return m_Boundary.GetCenterPoint(); }
	inline bool IsHit() const { return m_Hit; }


private:
	Triangle m_Boundary;
	Vec2D m_Direction;
	bool m_Hit;
};
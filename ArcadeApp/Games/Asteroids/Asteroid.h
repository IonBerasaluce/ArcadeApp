#pragma once
#include "Shapes/Circle.h"

class Screen;
class AARectangle;

class Asteroid
{
public:
	Asteroid();
	void Init(const Vec2D& position, const Vec2D& direction, unsigned int size);
	void Draw(Screen& screen);
	void Update(uint32_t dt, const AARectangle& boundary);

	void WrapAroundBoundary(const AARectangle& boundary);

	inline const Vec2D GetPosition() const { return m_Boundary.GetCenterPoint(); }
	inline const Vec2D GetDirection() const { return m_Direction; }
	inline const unsigned int GetSize() const { return m_Size; }
	inline bool IsDestroyed() const { return m_Destroyed; }

	void Hit();
	inline float const GetRadious() const { return m_Boundary.GetRadius(); }

private:

	Circle m_Boundary;
	Vec2D m_Direction;
	unsigned int m_Size;
	bool m_Destroyed;
	static const float m_MovementSpeed;
};
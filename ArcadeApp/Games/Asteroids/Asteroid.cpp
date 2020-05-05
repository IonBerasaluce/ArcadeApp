#include "Asteroid.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"

const float Asteroid::m_MovementSpeed = 0.05f;

Asteroid::Asteroid():m_Size(0), m_Destroyed(false)
{
}

void Asteroid::Init(const Vec2D& position, const Vec2D& direction, unsigned int size)
{
	m_Size = size;
	m_Direction = direction;

	m_Boundary = Circle(position, m_Size * 8);
	m_Destroyed = false;
}

void Asteroid::Draw(Screen& screen)
{
	screen.Draw(m_Boundary, Colour::White());
}

void Asteroid::Update(uint32_t dt, const AARectangle& boundary)
{
	m_Boundary.MoveBy(m_Direction.GetUnitVec() * dt * m_MovementSpeed);
	WrapAroundBoundary(boundary);
}

void Asteroid::WrapAroundBoundary(const AARectangle& boundary)
{
	Vec2D centrePoint = m_Boundary.GetCenterPoint();
	Vec2D position = centrePoint;

	if (centrePoint.GetX() < boundary.GetTopLeft().GetX())
	{
		position += Vec2D(boundary.GetWidth(), 0);
	}
	if (centrePoint.GetX() >= boundary.GetBottomRight().GetX())
	{
		position -= Vec2D(boundary.GetWidth(), 0);
	}
	if (centrePoint.GetY() < boundary.GetTopLeft().GetY())
	{
		position += Vec2D(0, boundary.GetHeight());
	}
	if (centrePoint.GetY() >= boundary.GetBottomRight().GetY())
	{
		position -= Vec2D(0, boundary.GetHeight());
	}

	m_Boundary.MoveTo(position);
}

void Asteroid::Hit()
{
	m_Destroyed = true;
}

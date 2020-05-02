#include "Misile.h"
#include "Graphics/Screen.h"

namespace 
{
	uint32_t MISILE_WIDTH = 10;
	uint32_t MISILE_HEIGHT = 10;
	uint32_t MISILE_SPEED = 2;
}

void Misile::Init(const Vec2D& direction, const Vec2D& position)
{
	m_Direction = direction;
	m_Boundary = Triangle(position, MISILE_WIDTH, MISILE_HEIGHT);

	// Update the direction of the boundary box
	Vec2D currentDirection = m_Boundary.GetP0() - m_Boundary.GetCenterPoint();
	float angle = currentDirection.AngleBetween360(m_Direction);
	m_Boundary.Rotate(angle);
}

void Misile::Update(uint32_t dt, const AARectangle& mapBoundary)
{
	Vec2D velocity = m_Direction * MISILE_SPEED;
	m_Boundary.MoveBy(velocity);
}

void Misile::Draw(Screen& screen)
{
	screen.Draw(m_Boundary, Colour::Red(), false);
}

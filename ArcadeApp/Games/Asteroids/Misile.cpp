#include "Misile.h"
#include "Graphics/Screen.h"

namespace 
{
	uint32_t MISILE_WIDTH = 2;
	uint32_t MISILE_HEIGHT = 5;
	uint32_t MISILE_SPEED = 2;
}


void Misile::Init(const Vec2D& direction, const Vec2D& position)
{
	m_Direction = direction;
	m_Boundary = Triangle(position, MISILE_WIDTH, MISILE_HEIGHT);
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

#include "Asteroid.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"

const float Asteroid::m_RotatingSpeed = 0.002f;
const float Asteroid::m_Speed = 0.008f;

Asteroid::Asteroid():m_Size(AsteroidSize::SMALL), m_Destroyed(false), m_Rotation(0), m_Reproduce(true)
{
}

void Asteroid::Init(const SpriteSheet& spriteSheet, const Vec2D& direction, const Vec2D& position, AsteroidSize size, const Colour& spriteColour)
{
	m_Size = size;
	m_SpriteSheet = spriteSheet;
	m_MovementDirection = direction;

	m_Sprite = spriteSheet.GetSprite(GetSpriteName());
	m_CollisionBoundary = Circle(position + m_Sprite.Offset(), (float)(m_Sprite.width / 2));
}

void Asteroid::Draw(Screen& screen)
{
	// We only rotate when we draw to the screen.
	screen.Draw(m_SpriteSheet, GetSpriteName(), m_CollisionBoundary.GetCenterPoint() + m_Sprite.Offset(), Colour::White(), m_Rotation);
}

void Asteroid::Update(uint32_t dt, const AARectangle& boundary)
{
	m_CollisionBoundary.MoveBy(m_MovementDirection * m_Speed * (float)dt);
	m_Rotation += m_RotatingSpeed * dt;

	WrapAroundBoundary(boundary);
}

void Asteroid::Hit(const bool split)
{
	m_Destroyed = true;
	m_Reproduce = split;
}


std::string Asteroid::GetSpriteName()
{
	std::string spriteName;

	switch (m_Size)
	{
		case SMALL:
		{
			spriteName = "small_rock";
			return spriteName;
		}
		case MEDIUM:
		{
			spriteName = "medium_rock";
			return spriteName;
		}
		case LARGE:
		{
			spriteName = "medium_rock2";
			return spriteName;
		}
		case EXTRALARGE:
		{
			spriteName = "big_rock";
			return spriteName;
		}
	}

	return spriteName;
}


void Asteroid::WrapAroundBoundary(const AARectangle& boundary)
{
	Vec2D centrePoint = m_CollisionBoundary.GetCenterPoint();
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

	m_CollisionBoundary.MoveTo(position);
}
#include <string>

#include "Asteroid.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"
#include "Graphics/AnimatedSprite.h"
#include "App/App.h"

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
	m_SpriteBox = AARectangle(position, m_Sprite.width, m_Sprite.height);
	m_CollisionBoundary = Circle(m_SpriteBox.GetCenterPoint(), (float)(m_Sprite.width / 2));
}

void Asteroid::Draw(Screen& screen)
{
	screen.Draw(m_SpriteSheet, GetSpriteName(), m_SpriteBox.GetTopLeft(), Colour::White(), m_Rotation);
}

void Asteroid::Update(uint32_t dt, const AARectangle& boundary)
{
	m_SpriteBox.MoveBy(m_MovementDirection * m_Speed * (float)dt);
	m_CollisionBoundary.MoveTo(m_SpriteBox.GetCenterPoint());
	m_Rotation += m_RotatingSpeed * dt;

	WrapAroundBoundary(boundary);
}

int const Asteroid::GetScore() const
{
	
	switch (m_Size)
	{
	case SMALL:
		return 100;
		break;
	case MEDIUM:
		return 50;
		break;
	case LARGE:
		return 50;
		break;
	case EXTRALARGE:
		return 10;
		break;
	case NONE:
		return 0;
	default:
		break;
	}
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
		case AsteroidSize::SMALL:
		{
			spriteName = "small_rock";
			return spriteName;
		}
		case AsteroidSize::MEDIUM:
		{
			spriteName = "medium_rock";
			return spriteName;
		}
		case AsteroidSize::LARGE:
		{
			spriteName = "medium_rock2";
			return spriteName;
		}
		case AsteroidSize::EXTRALARGE:
		{
			spriteName = "big_rock";
			return spriteName;
		}
	}

	return spriteName;
}


void Asteroid::WrapAroundBoundary(const AARectangle& boundary)
{
	Vec2D topLeft = m_SpriteBox.GetTopLeft();
	Vec2D position = topLeft;

	if (topLeft.GetX() < boundary.GetTopLeft().GetX())
	{
		position += Vec2D(boundary.GetWidth(), 0);
	}
	if (topLeft.GetX() >= boundary.GetBottomRight().GetX())
	{
		position -= Vec2D(boundary.GetWidth(), 0);
	}
	if (topLeft.GetY() < boundary.GetTopLeft().GetY())
	{
		position += Vec2D(0, boundary.GetHeight());
	}
	if (topLeft.GetY() >= boundary.GetBottomRight().GetY())
	{
		position -= Vec2D(0, boundary.GetHeight());
	}

	m_SpriteBox.MoveTo(position);
}
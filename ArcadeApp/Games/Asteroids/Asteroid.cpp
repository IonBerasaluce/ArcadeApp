#include "Asteroid.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"

const float Asteroid::m_RotatingSpeed = 0.001f;
const float Asteroid::m_Speed = 0.005f;

Asteroid::Asteroid():m_Size(AsteroidSize::SMALL), m_Destroyed(false), m_Rotation(0)
{
}

void Asteroid::Init(const SpriteSheet& spriteSheet, const Vec2D& direction, const Vec2D& position, AsteroidSize size, const Colour& spriteColour)
{
	m_Size = size;
	m_SpriteSheet = spriteSheet;
	m_MovementDirection = direction;

	m_Sprite = spriteSheet.GetSprite(GetSpriteName());
	m_CollisionBoundary = Circle(position + m_Sprite.Offset(), m_Sprite.width / 2);
}

void Asteroid::Draw(Screen& screen)
{
	// We only rotate when we draw to the screen.
	screen.Draw(m_SpriteSheet, GetSpriteName(), m_CollisionBoundary.GetCenterPoint() + m_Sprite.Offset(), Colour::White(), m_Rotation);
}

void Asteroid::Update(uint32_t dt, const AARectangle& boundary)
{
	m_CollisionBoundary.MoveBy(m_MovementDirection * m_Speed * dt);
	m_Rotation += m_RotatingSpeed * dt;
}

void Asteroid::Hit()
{
	m_Destroyed = true;
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
}

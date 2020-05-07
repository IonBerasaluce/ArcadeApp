#include "Asteroid.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"

const float Asteroid::m_RotatingSpeed = 0.001f;

Asteroid::Asteroid():m_Size(AsteroidSize::SMALL), m_Destroyed(false), m_Rotation(0)
{
}

void Asteroid::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& direction, const Vec2D& position, AsteroidSize size, const Colour& spriteColour)
{
	AsteroidsActor::Init(spriteSheet, animationsPath, position, 0.0f, spriteColour);
	m_Size = size;
	m_MovementDirection = direction;
	m_Speed = 0.005f;
	m_CollisionBoundary = Circle(m_Sprite.GetBoundingBox().GetCenterPoint(), m_Sprite.GetBoundingBox().GetHeight() / 2);
}

void Asteroid::Draw(Screen& screen)
{
	// We only rotate when we draw to the screen.
	screen.Draw(*m_Sprite.GetSpriteSheet(), GetSprite(), m_Sprite.Position(), Colour::White(), m_Rotation);
}

void Asteroid::Update(uint32_t dt, const AARectangle& boundary)
{
	AsteroidsActor::Update(dt);
	
	WrapAroundBoundary(boundary);
	m_CollisionBoundary.MoveTo(m_Sprite.GetBoundingBox().GetCenterPoint());

	m_Rotation += m_RotatingSpeed * dt;
}

void Asteroid::Hit()
{
	m_Destroyed = true;
}


std::string Asteroid::GetSprite()
{
	switch (m_Size)
	{
		case SMALL:
		{
			std::string spriteName = "small_rock";
			return spriteName;
		}
		case MEDIUM:
		{
			std::string spriteName = "medium_rock";
			return spriteName;
		}
		case LARGE:
		{
			std::string spriteName = "medium_rock2";
			return spriteName;
		}
	}
}

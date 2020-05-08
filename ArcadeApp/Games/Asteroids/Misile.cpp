#include "Misile.h"
#include "Graphics/Screen.h"

namespace 
{
	uint32_t MISILE_WIDTH = 10;
	uint32_t MISILE_HEIGHT = 10;
	float MISILE_SPEED = 0.15f;
}

Misile::Misile() :m_Hit(false), m_Exploding(false)
{
}

void Misile::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& direction, const Vec2D& position, const Colour& spriteColour)
{
	AsteroidsActor::Init(spriteSheet, animationsPath, position, MISILE_SPEED, spriteColour);
	m_LookingDirection = direction;
	m_MovementDirection = direction;
	m_CollisionBoundary = Circle(Vec2D::Zero, m_Sprite.GetBoundingBox().GetHeight() / 2);
	m_Hit = false;
	SetFirstAnimation();
}

void Misile::Update(uint32_t dt, const AARectangle& mapBoundary)
{
	if (m_Exploding)
	{
		m_Sprite.Update(dt);
		if (IsFinishedAnimation())
		{
			m_Exploding = false;
			MoveTo(Vec2D(-100.0f, 0.0f));
		}
		return;
	}

	Vec2D velocity = m_MovementDirection * MISILE_SPEED;
	AsteroidsActor::Update(dt);
	m_CollisionBoundary.MoveTo(m_Sprite.GetBoundingBox().GetCenterPoint());
}

void Misile::Hit(bool exploding)
{
	m_Hit = true;
	if (exploding)
	{
		SetAnimation("explosion", false);
		m_Exploding = true;
	}
	else
	{
		MoveTo(Vec2D(-100.0f, 0.0f));
	}
	
}

void Misile::SetFirstAnimation()
{
	SetAnimation("missile", true);
}

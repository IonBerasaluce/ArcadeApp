#include "Misile.h"
#include "Graphics/Screen.h"

namespace 
{
	uint32_t MISILE_WIDTH = 10;
	uint32_t MISILE_HEIGHT = 10;
	float MISILE_SPEED = 0.1;
}

Misile::Misile() :m_Hit(false)
{
}

void Misile::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& direction, const Vec2D& position, const Colour& spriteColour)
{
	AsteroidsActor::Init(spriteSheet, animationsPath, position, MISILE_SPEED, spriteColour);
	m_Hit = false;
	m_LookingDirection = direction;
	m_MovementDirection = direction;
	SetFirstAnimation();
}

void Misile::Update(uint32_t dt, const AARectangle& mapBoundary)
{
	Vec2D velocity = m_MovementDirection * MISILE_SPEED;
	AsteroidsActor::Update(dt);
}

void Misile::Draw(Screen& screen)
{
	AsteroidsActor::Draw(screen);
}

void Misile::Hit()
{
	m_Hit = true;
	MoveTo(Vec2D(-1000, 0));
}

void Misile::SetFirstAnimation()
{
	SetAnimation("missile", true);
}

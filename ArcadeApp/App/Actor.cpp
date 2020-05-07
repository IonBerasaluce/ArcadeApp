
#include "Actor.h"
#include "Math/Vec2D.h"

void Actor::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, const Colour& spriteColour)
{
	m_Sprite.Init(animationsPath, spriteSheet, spriteColour);
	m_Sprite.SetPosition(initialPos);
}

void Actor::Draw(Screen& screen)
{
	m_Sprite.Draw(screen);
}

void Actor::SetAnimation(const std::string& animationName, bool looped)
{
	m_Sprite.SetAnimation(animationName, looped);
}
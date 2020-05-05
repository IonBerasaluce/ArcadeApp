#include "Actor.h"


void Actor::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, const Colour& spriteColour)
{
	m_Sprite.Init(animationsPath, spriteSheet, spriteColour);
	m_Sprite.SetPosition(initialPos);
}
#pragma once
#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"
#include "AsteroidsActor.h"

class Screen;

class Misile : public AsteroidsActor
{
public:

	Misile();
	void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& direction, const Vec2D& position, const Colour& spriteColour = Colour::White());
	void Update(uint32_t dt, const AARectangle& boundary);
	inline Circle const GetCollisionBox() const { return m_CollisionBoundary; }

	void Hit(bool exploding);
	void SetFirstAnimation();
	inline bool IsHit() const { return m_Hit; }

private:
	bool m_Hit;
	bool m_Exploding;
	Circle m_CollisionBoundary;
};
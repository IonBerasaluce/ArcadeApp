#pragma once
#include "Shapes/Circle.h"
#include "Graphics/SpriteSheet.h"
#include "AsteroidsActor.h"

class Screen;
class AARectangle;

enum AsteroidSize
{
	SMALL = 0,
	MEDIUM,
	LARGE
};

class Asteroid : public AsteroidsActor
{
public:
	Asteroid();
	void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& direction, const Vec2D& position, AsteroidSize size, const Colour& spriteColour = Colour::White());
	void Draw(Screen& screen);
	void Update(uint32_t dt, const AARectangle& boundary);

	inline const unsigned int GetSize() const { return static_cast<int>(m_Size); }
	inline bool IsDestroyed() const { return m_Destroyed; }

	void Hit();
	inline float const GetRadious() const { return m_CollisionBoundary.GetRadius(); }

private:
	std::string GetSprite();

private:
	Circle m_CollisionBoundary;
	Vec2D m_LookingDirection;
	AsteroidSize m_Size;
	bool m_Destroyed;
	float m_Rotation;

	static const float m_RotatingSpeed;
};
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
	LARGE,
	EXTRALARGE
};

class Asteroid
{
public:
	Asteroid();
	void Init(const SpriteSheet& spriteSheet, const Vec2D& direction, const Vec2D& position, AsteroidSize size, const Colour& spriteColour = Colour::White());
	void Draw(Screen& screen);
	void Update(uint32_t dt, const AARectangle& boundary);

	inline const unsigned int GetSize() const { return static_cast<int>(m_Size); }
	inline bool IsDestroyed() const { return m_Destroyed; }
	inline float const GetRadious() const { return m_CollisionBoundary.GetRadius(); }
	inline Vec2D const Position() const { return m_CollisionBoundary.GetCenterPoint(); }
	inline Circle const GetCollisionBox() const{ return m_CollisionBoundary; }
	inline bool Reproduce() const { return m_Reproduce; }

	void Hit(const bool split = true);

private:
	std::string GetSpriteName();
	void WrapAroundBoundary(const AARectangle& boundary);

private:
	Circle m_CollisionBoundary;
	Vec2D m_LookingDirection;
	Vec2D m_MovementDirection;
	
	AsteroidSize m_Size;
	bool m_Destroyed;
	float m_Rotation;
	bool m_Reproduce;

	SpriteSheet m_SpriteSheet;
	Sprite m_Sprite;

	static const float m_RotatingSpeed;
	static const float m_Speed;
};
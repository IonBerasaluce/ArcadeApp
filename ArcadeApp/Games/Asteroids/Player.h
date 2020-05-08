#pragma once
#include <vector>

#include "Shapes/Triangle.h"
#include "AsteroidsActor.h"

class Screen;
class AARectangle;

enum class RotationDirection
{
	ROTATE_LEFT = -1,
	ROTATE_RIGHT = 1
};

enum class AsteroidsPlayerStates
{
	ALIVE = 0,
	DEAD,
	DYING
};


class Player : public AsteroidsActor
{
public:

	Player();
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Colour& spriteColour = Colour::White());
	virtual void Update(uint32_t dt, const AARectangle& boundary);

	void Accelerate(uint32_t dt);
	void Rotate(RotationDirection rotationDirection);
	void MoveTo(const Vec2D& position);
	void Reset();
	void CrashedIntoAsteroid();

	inline void LossLife() { --m_Lives; }
	inline Circle const GetCollisionBox() const { return m_CollisionBoundary; }

private:
	void ResetScore();
	void ResetDirection();
	void ResetToFirstAnimation();

private:
	int m_Lives;
	int m_Score;
	AsteroidsPlayerStates m_PlayerState;
	Circle m_CollisionBoundary;
	static const float PLAYER_ACCELERATION;
	static const float MAX_SPEED;
};
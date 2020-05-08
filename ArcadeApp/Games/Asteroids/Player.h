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


class Player : public AsteroidsActor
{
public:

	Player();
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Colour& spriteColour = Colour::White());
	virtual void Update(uint32_t dt, const AARectangle& boundary);
	virtual void SetAnimation(const std::string& animationName, bool looped) override;

	void Accelerate(uint32_t dt);
	void Rotate(RotationDirection rotationDirection);
	void MoveTo(const Vec2D& position);
	void Reset();
	virtual void Draw(Screen& screen) override;

	inline void LossLife() { --m_Lives; }
	

private:
	void ResetScore();
	void ResetDirection();

private:
	int m_Lives;
	int m_Score;
	Circle m_CollisionBoundary;
	static const float PLAYER_ACCELERATION;
	static const float MAX_SPEED;
};
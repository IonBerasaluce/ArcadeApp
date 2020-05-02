#pragma once
#include <vector>

#include "Shapes/Triangle.h"
#include "Misile.h"

class Screen;
class AARectangle;

enum class RotationDirection
{
	ROTATE_LEFT = -1,
	ROTATE_RIGHT = 1
};


class Player
{
public:

	Player();
	void Init(Vec2D playerPos);
	void Draw(Screen& screen);
	void Update(uint32_t dt, const AARectangle& boundary);
	
	void Accelerate(uint32_t dt);
	void Rotate(RotationDirection rotationDirection);

	inline Vec2D GetPosition() const { return m_bTriangle.GetCenterPoint(); }
	void ShootMissile();

	void WrapAroundBoundary(const AARectangle& boundary);


private:
	Triangle m_bTriangle;
	float m_CurrentSpeed;
	Vec2D m_CurrentVelocity;
	std::vector<Misile> m_Misiles;

private:
	Vec2D GetCurrentDirection() const;


private:
	static const unsigned int PLAYER_WIDTH = 12;
	static const unsigned int PLAYER_HEIGHT = 20;
	
	static const float PLAYER_ACCELERATION;
	static const float MAX_SPEED;
};
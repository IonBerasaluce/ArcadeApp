#pragma once
#include "Shapes/Triangle.h"

class Screen;

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
	void Update(uint32_t dt);
	
	void Accelerate(uint32_t dt);
	void Rotate(RotationDirection rotationDirection);

	inline Vec2D GetPosition() const { return m_bTriangle.GetCenterPoint(); }
	void ShootMissile();


private:
	Triangle m_bTriangle;
	
	// Direction 
	float m_CurrentSpeed;
	Vec2D m_CurrentVelocity;

	Vec2D GetCurrentDirection() const;
	
	static const unsigned int PLAYER_WIDTH = 12;
	static const unsigned int PLAYER_HEIGHT = 20;
	
	static const float PLAYER_ACCELERATION;
	static const float PLAYER_ROTATIONAL_ACCELERATION;
	static const float MAX_SPEED;
};
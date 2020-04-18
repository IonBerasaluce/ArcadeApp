#pragma once
#include <stdint.h>

#include "Excluder.h"

class Screen;
class Ball;

enum PaddleDirection
{
	LEFT = 1 << 0,
	RIGHT = 1 << 1
};

class Paddle : public Excluder
{
public:

	static const uint32_t PADDLE_WIDTH = 50;
	static const uint32_t PADDLE_HEIGHT = 10;

	void Init(const AARectangle& rect, const AARectangle& boundary);
	void Update(uint32_t dt, Ball& ball);
	void Draw(Screen& screen);
	bool Bounce(Ball& Ball);
	
	inline bool IsMovingLeft() const { return m_Direction == PaddleDirection::LEFT; }
	inline bool IsMovingRight() const { return m_Direction == PaddleDirection::RIGHT; }

	inline void SetMovementDirection(PaddleDirection dir) { m_Direction |= dir; }
	inline void UnSetMovementDirection(PaddleDirection dir) { m_Direction &= ~dir; }
	inline void StopMovement() { m_Direction = 0; }

private:
	uint32_t m_Direction;
	AARectangle m_Boundary;
	const float VELOCITY = 100.0;
	const float CORNER_BOUNCE_AMT = 0.2F;

};
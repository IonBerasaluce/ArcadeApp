#pragma once

#pragma once

#include <string>

#include "Math/Vec2D.h"
#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Graphics/Colour.h"
#include "App/Actor.h"

class Screen;
class SpriteSheet;

class AsteroidsActor : public Actor
{
public:

	virtual ~AsteroidsActor() {}
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, float movementSpeed, const Colour& spriteColour = Colour::White());
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;

	void WrapAroundBoundary(const AARectangle& boundary);
	void Rotate(const float angle);

	inline void SetMovementDirection(Vec2D direction) { m_MovementDirection = direction; }
	inline void SetMovementSpeed(const float speed) { m_Speed = speed; }
	inline Vec2D GetLookingDirection() const { return m_LookingDirection; }
	inline void Stop() { m_Speed = 0.0f; }

protected:
	float m_Speed;
	Vec2D m_MovementDirection;
	Vec2D m_LookingDirection;

private:
	float m_RotationAngle;
	Vec2D m_Offset;
};
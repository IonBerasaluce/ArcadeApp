#pragma once

#include <string>

#include "Graphics/AnimatedSprite.h"
#include "Math/Vec2D.h"
#include "PacmanGameUtils.h"
#include "Shapes/AARectangle.h"
#include "Graphics/Colour.h"
#include "App/Actor.h"

class Screen;
class SpriteSheet;

class PacmanActor : public Actor
{
public:

	virtual ~PacmanActor() {}
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Colour& spriteColour = Colour::White());
	
	virtual void Update(uint32_t dt) override;
	virtual void Stop();

	virtual inline void SetMovementDirection(PacmanMovement direction) { m_MovementDirection = direction; }

	AARectangle GetEatingBoundingBox() const;

	inline bool IsFinishedAnimation() const { return m_Sprite.IsFinishedPlayingAnimation(); }
	inline const AARectangle GetBoundingBox() const { return m_Sprite.GetBoundingBox(); }
	inline void MoveBy(const Vec2D& delta) { m_Sprite.MoveBy(delta); }
	inline void MoveTo(const Vec2D& position) { m_Sprite.SetPosition(position); }
	inline PacmanMovement GetMovementDirection() const { return m_MovementDirection; }

protected:	
	inline void ResetDelta() { m_Delta = Vec2D::Zero; }
	inline void SetMovementSpeed(uint32_t movementSpeed) { m_MovementSpeed = movementSpeed; }

private:
	Vec2D m_Delta;
	PacmanMovement m_MovementDirection;
	uint32_t m_MovementSpeed;
	bool m_UpdateSpriteOnUpdate;
};
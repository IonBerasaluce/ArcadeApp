#pragma once

#include <string>

#include "Graphics/AnimatedSprite.h"
#include "Math/Vec2D.h"
#include "PacmanGameUtils.h"
#include "Shapes/AARectangle.h"
#include "Graphics/Colour.h"

class Screen;
class SpriteSheet;

class Actor
{
public:

	virtual ~Actor() {}
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Colour& spriteColour = Colour::White());
	
	virtual void Update(uint32_t dt);
	virtual void Draw(Screen& screen);
	virtual void Stop();
	virtual inline void SetMovementDirection(PacmanMovement direction) { m_MovementDirection = direction; }

	AARectangle GetEatingBoundingBox() const;

	inline bool IsFinishedAnimation() const { return m_Sprite.IsFinishedPlayingAnimation(); }
	inline const AARectangle GetBoundingBox() const { return m_Sprite.GetBoundingBox(); }
	inline void MoveBy(const Vec2D& delta) { m_Sprite.MoveBy(delta); }
	inline void MoveTo(const Vec2D& position) { m_Sprite.SetPosition(position); }
	inline Vec2D Position() const { return m_Sprite.Position(); }
	inline PacmanMovement GetMovementDirection() const { return m_MovementDirection; }
	inline const Colour& GetSpriteColour() const { return m_Sprite.GetColour(); }


protected:
	void SetAnimation(const std::string& animationName, bool looped);
	
	inline void ResetDelta() { m_Delta = Vec2D::Zero; }
	inline void SetMovementSpeed(uint32_t movementSpeed) { m_MovementSpeed = movementSpeed; }

	AnimatedSprite m_Sprite;

private:
	Vec2D m_Delta;
	PacmanMovement m_MovementDirection;
	uint32_t m_MovementSpeed;
	bool m_UpdateSpriteOnUpdate;
};
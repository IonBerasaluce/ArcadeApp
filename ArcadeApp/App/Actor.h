#pragma once

#include "Graphics/AnimatedSprite.h"
#include "Math/Vec2D.h"
#include "Shapes/AARectangle.h"
#include "Graphics/Colour.h"

class Screen;
class SpriteSheet;

class Actor
{
public:

	virtual ~Actor() {}
	void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, const Colour& spriteColour = Colour::White());

	virtual void Update(uint32_t dt) = 0;
	virtual void Draw(Screen& screen) = 0;
	virtual void Stop() = 0;

	inline bool IsFinishedAnimation() const { return m_Sprite.IsFinishedPlayingAnimation(); }
	
	inline void MoveBy(const Vec2D& delta) { m_Sprite.MoveBy(delta); }
	inline void MoveTo(const Vec2D& position) { m_Sprite.SetPosition(position); }
	inline Vec2D Position() const { return m_Sprite.Position(); }
	inline const Colour& GetSpriteColour() const { return m_Sprite.GetColour(); }

protected:
	inline void SetAnimation(const std::string& animationName, bool looped)	{ m_Sprite.SetAnimation(animationName, looped); }
	AnimatedSprite m_Sprite;

};

#pragma once

#include <string>

#include "Graphics/AnimatedSprite.h"
#include "Shapes/AARectangle.h"
#include "Graphics/Colour.h"

class Screen;
class SpriteSheet;
class Vec2D;

class Actor
{
public:

	virtual ~Actor() {}
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, const Colour& spriteColour = Colour::White());

	virtual void Update(uint32_t dt) = 0;
	virtual void Draw(Screen& screen);

	inline bool IsFinishedAnimation() const { return m_Sprite.IsFinishedPlayingAnimation(); }

	inline Vec2D Position() const { return m_Sprite.Position(); }
	inline const Colour& GetSpriteColour() const { return m_Sprite.GetColour(); }

	inline void MoveBy(const Vec2D& delta) { m_Sprite.MoveBy(delta); }
	inline void MoveTo(const Vec2D& position) { m_Sprite.SetPosition(position); }

	inline const AARectangle GetBoundingBox() const { return m_Sprite.GetBoundingBox(); }

protected:
	void SetAnimation(const std::string& animationName, bool looped);

	AnimatedSprite m_Sprite;

};
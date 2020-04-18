#pragma once

#include "AnimationPlayer.h"
#include "Math/Vec2D.h"
#include "Colour.h"

class AARectangle;
class Screen;
class SpriteSheet;


class AnimatedSprite
{
public:
	AnimatedSprite();
	void Init(const std::string& animationsPath, const SpriteSheet& spriteSheet, const Colour& colour = Colour::White());
	void Update(uint32_t dt);
	void Draw(Screen& theScreen);

	void SetAnimation(const std::string& animationName, bool looped);
	Vec2D Size() const;
	void Pause();
	const AARectangle GetBoundingBox() const;

	inline Vec2D Position() const { return m_Position; }
	inline void SetPosition(const Vec2D position) { m_Position = position; }
	inline void MoveBy(const Vec2D& delta) { m_Position += delta; }
	inline bool IsFinishedPlayingAnimation() const { return m_AnimationPlayer.IsFinishedPlaying(); }
	inline const Colour& GetColour() const{ return m_Colour; }
	const SpriteSheet* GetSpriteSheet() const { return m_ptrSpriteSheet; }


private:
	const SpriteSheet* m_ptrSpriteSheet;
	AnimationPlayer m_AnimationPlayer;
	Vec2D m_Position;
	Colour m_Colour;
};
#include "AnimatedSprite.h"
#include "Shapes/AARectangle.h"
#include "Screen.h"

AnimatedSprite::AnimatedSprite():m_Position(Vec2D::Zero), m_ptrSpriteSheet(nullptr)
{
}

void AnimatedSprite::Init(const std::string& animationsPath, const SpriteSheet& spriteSheet, const Colour& colour)
{
	m_AnimationPlayer.Init(animationsPath);
	m_ptrSpriteSheet = &spriteSheet;
	m_Colour = colour;

}

void AnimatedSprite::Update(uint32_t dt)
{
	m_AnimationPlayer.Update(dt);
}

void AnimatedSprite::Draw(Screen& theScreen)
{
	AnimationFrame frame = m_AnimationPlayer.GetCurrentAnimationFrame();

	Colour frameColour = frame.frameColour;

	if (!frame.frameColourSet)
	{
		frameColour = m_Colour;
	}

	theScreen.Draw(*m_ptrSpriteSheet, frame.frame, m_Position + frame.offset, frameColour);

	if (frame.overlay.size() > 0)
	{
		theScreen.Draw(*m_ptrSpriteSheet, frame.overlay, m_Position, frame.overlayColour);
	}
}

void AnimatedSprite::SetAnimation(const std::string& animationName, bool looped)
{
	m_AnimationPlayer.Play(animationName, looped);
}

Vec2D AnimatedSprite::Size() const
{
	return m_AnimationPlayer.GetCurrentAnimationFrame().size;
}

void AnimatedSprite::Stop()
{
	m_AnimationPlayer.Stop();
}

const AARectangle AnimatedSprite::GetBoundingBox() const
{
	AARectangle bbox = { m_Position, static_cast<unsigned int>(Size().GetX()), static_cast<unsigned int>(Size().GetY()) };
	return bbox;
}

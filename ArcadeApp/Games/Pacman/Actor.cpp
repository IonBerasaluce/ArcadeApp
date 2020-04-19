#include "Actor.h"

void Actor::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Colour& spriteColour)
{
	m_MovementDirection = PACMAN_MOVE_NONE;
	m_Sprite.Init(animationsPath, spriteSheet, spriteColour);
	m_UpdateSpriteOnUpdate = updateSpriteOnMovement;
	m_MovementSpeed = movementSpeed;
	m_Delta = Vec2D::Zero;
	m_Sprite.SetPosition(initialPos);
}

void Actor::Update(uint32_t dt)
{
	if (m_MovementDirection != PACMAN_MOVE_NONE)
	{
		Vec2D delta = Vec2D::Zero;

		delta = GetMovementVector(m_MovementDirection) * m_MovementSpeed;

		m_Delta += delta * MillisecondsToSeconds(dt);
		
		if (fabsf(m_Delta.GetX()) >= 1)
		{
			int dx = int(fabs(m_Delta.GetX()));
			if (m_Delta.GetX() < 0)
			{
				m_Sprite.MoveBy(Vec2D(-dx, 0));
				m_Delta.SetX(m_Delta.GetX() + dx);
			}
			else
			{
				m_Sprite.MoveBy(Vec2D(dx, 0));
				m_Delta.SetX(m_Delta.GetX() - dx);
			}
		}
		else if (fabsf(m_Delta.GetY()) >= 1)
		{
			int dy = int(fabsf(m_Delta.GetY()));

			if (m_Delta.GetY() < 0)
			{
				m_Sprite.MoveBy(Vec2D(0, -dy));
				m_Delta.SetY(m_Delta.GetY() + dy);
			}
			else
			{
				m_Sprite.MoveBy(Vec2D(0, dy));
				m_Delta.SetY(m_Delta.GetY() - dy);
			}
		}
		m_Sprite.Update(dt);
	}
	// Handling moving actors during paused stated
	if (m_UpdateSpriteOnUpdate && m_MovementDirection == PACMAN_MOVE_NONE)
	{
		m_Sprite.Update(dt);
	}
}

void Actor::Draw(Screen& screen)
{
	m_Sprite.Draw(screen);
}

void Actor::Stop()
{
	SetMovementDirection(PACMAN_MOVE_NONE);
	m_Sprite.Stop();
}

AARectangle Actor::GetEatingBoundingBox() const
{
	return AARectangle::Inset(GetBoundingBox(), Vec2D(3, 3));
}

void Actor::SetAnimation(const std::string& animationName, bool looped)
{
	m_Sprite.SetAnimation(animationName, looped);
}

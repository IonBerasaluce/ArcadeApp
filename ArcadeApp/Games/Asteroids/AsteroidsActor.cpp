#include "AsteroidsActor.h"
#include "Graphics/Screen.h"
#include "Shapes/Line2D.h"

AsteroidsActor::AsteroidsActor():m_Speed(0.0f), m_RotationAngle(0.0f)
{
}

void AsteroidsActor::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, float movementSpeed, const Colour& spriteColour)
{
	Actor::Init(spriteSheet, animationsPath, initialPos, spriteColour);
	m_MovementDirection = Vec2D(0, -1);
	m_LookingDirection = Vec2D(0, -1);
	m_Speed = movementSpeed;
	m_RotationAngle = 0;	
}

void AsteroidsActor::Update(uint32_t dt)
{
	Vec2D velocity = m_MovementDirection * (float)dt * m_Speed;
	m_Sprite.MoveBy(velocity);
	m_Sprite.Update(dt);
}

void AsteroidsActor::Draw(Screen& screen)
{
	float rotationAngle = m_LookingDirection.AngleBetween360(Vec2D(0, -1));

	m_Sprite.Draw(screen, -1 * rotationAngle);
}

void AsteroidsActor::WrapAroundBoundary(const AARectangle& boundary)
{
	Vec2D topLeft = m_Sprite.GetBoundingBox().GetTopLeft();
	Vec2D position = topLeft;

	if (topLeft.GetX() < boundary.GetTopLeft().GetX())
	{
		position += Vec2D(boundary.GetWidth(), 0);
	}
	if (topLeft.GetX() >= boundary.GetBottomRight().GetX())
	{
		position -= Vec2D(boundary.GetWidth(), 0);
	}
	if (topLeft.GetY() < boundary.GetTopLeft().GetY())
	{
		position += Vec2D(0, boundary.GetHeight());
	}
	if (topLeft.GetY() >= boundary.GetBottomRight().GetY())
	{
		position -= Vec2D(0, boundary.GetHeight());
	}

	m_Sprite.MoveTo(position);
}

void AsteroidsActor::Rotate(const float angle)
{
	// This is how the sprite should rotate
	Vec2D pointInBoundary = m_Sprite.GetBoundingBox().GetCenterPoint() + m_LookingDirection * m_Sprite.GetBoundingBox().GetHeight() / 2;
	pointInBoundary.Rotate(angle, m_Sprite.GetBoundingBox().GetCenterPoint());
	Vec2D direction = pointInBoundary - m_Sprite.GetBoundingBox().GetCenterPoint();

	m_LookingDirection = direction.GetUnitVec();
}

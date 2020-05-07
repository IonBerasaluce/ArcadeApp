#include "AsteroidsActor.h"

void AsteroidsActor::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, float movementSpeed, const Colour& spriteColour)
{
	Actor::Init(spriteSheet, animationsPath, initialPos, spriteColour);
	m_BoundingBox = Circle(initialPos, m_Sprite.GetBoundingBox().GetWidth() / 2);
	m_MovementDirection = Vec2D(0, -1);
	m_Speed = movementSpeed;
	m_RotationAngle = 0;	
}

void AsteroidsActor::Update(uint32_t dt)
{
	m_BoundingBox.MoveBy(m_MovementDirection * (float)dt * m_Speed);
}

void AsteroidsActor::Draw(Screen& screen)
{
	m_Sprite.Draw(screen, m_RotationAngle);
}

void AsteroidsActor::WrapAroundBoundary(const AARectangle& boundary)
{
	// This needs some cleaning 
	Vec2D centrePoint = m_BoundingBox.GetCenterPoint();
	Vec2D position = centrePoint;

	if (centrePoint.GetX() < boundary.GetTopLeft().GetX())
	{
		position += Vec2D(boundary.GetWidth(), 0);
	}
	if (centrePoint.GetX() >= boundary.GetBottomRight().GetX())
	{
		position -= Vec2D(boundary.GetWidth(), 0);
	}
	if (centrePoint.GetY() < boundary.GetTopLeft().GetY())
	{
		position += Vec2D(0, boundary.GetHeight());
	}
	if (centrePoint.GetY() >= boundary.GetBottomRight().GetY())
	{
		position -= Vec2D(0, boundary.GetHeight());
	}

	m_BoundingBox.MoveTo(position);
}

void AsteroidsActor::Rotate(const float angle)
{
	m_MovementDirection.Rotate(angle, m_BoundingBox.GetCenterPoint());
	m_RotationAngle += angle;
}

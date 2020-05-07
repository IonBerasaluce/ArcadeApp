#include "AsteroidsActor.h"
#include "Graphics/Screen.h"
#include "Shapes/Line2D.h"

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
}

void AsteroidsActor::Draw(Screen& screen)
{
	float rotationAngle = m_LookingDirection.AngleBetween360(Vec2D(0, -1));

	m_Sprite.Draw(screen, -1 * rotationAngle);
}

void AsteroidsActor::WrapAroundBoundary(const AARectangle& boundary)
{
	// This needs some cleaning 
	Vec2D centrePoint = m_Sprite.GetBoundingBox().GetTopLeft();
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

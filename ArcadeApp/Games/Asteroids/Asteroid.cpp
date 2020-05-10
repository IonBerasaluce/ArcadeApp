#include <string>

#include "Asteroid.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"
#include "Graphics/AnimatedSprite.h"
#include "App/App.h"

const float Asteroid::m_RotatingSpeed = 0.002f;
const float Asteroid::m_Speed = 0.008f;

Asteroid::Asteroid() :m_Size(AsteroidSize::SMALL), m_Destroyed(false), m_Rotation(0), m_Reproduce(true), m_Score(0)
{
}

void Asteroid::Init(const SpriteSheet& spriteSheet, const Vec2D& direction, const Vec2D& position, AsteroidSize size, const Colour& spriteColour)
{
	m_Size = size;
	m_SpriteSheet = spriteSheet;
	m_MovementDirection = direction;

	m_Sprite = spriteSheet.GetSprite(GetSpriteName());
	m_SpriteBox = AARectangle(position, m_Sprite.width, m_Sprite.height);

	Vec2D sWidth = Vec2D((float)App::Singleton().Width(), 0.0f);
	Vec2D sHeight = Vec2D(0.0f, (float)App::Singleton().Height());
	
	// We need to create 4 boundaries at each side of the screen
	m_CollisionBoundaries.push_back(Circle(m_SpriteBox.GetCenterPoint(), (float)(m_Sprite.width / 2)));
	m_CollisionBoundaries.push_back(Circle(m_SpriteBox.GetCenterPoint() - sWidth, (float)(m_Sprite.width / 2)));
	m_CollisionBoundaries.push_back(Circle(m_SpriteBox.GetCenterPoint() + sWidth, (float)(m_Sprite.width / 2)));
	m_CollisionBoundaries.push_back(Circle(m_SpriteBox.GetCenterPoint() - sHeight, (float)(m_Sprite.width / 2)));
	m_CollisionBoundaries.push_back(Circle(m_SpriteBox.GetCenterPoint() + sHeight, (float)(m_Sprite.width / 2)));
}

void Asteroid::Draw(Screen& screen)
{
	screen.Draw(m_SpriteSheet, GetSpriteName(), m_SpriteBox.GetTopLeft(), Colour::White(), m_Rotation);
}

void Asteroid::Update(uint32_t dt, const AARectangle& boundary)
{
	m_Rotation += m_RotatingSpeed * dt;
	Vec2D delta = (m_MovementDirection * m_Speed * (float)dt);

	// Update the sprite's location
	m_SpriteBox.MoveBy(delta);
	WrapAroundBoundary(&m_SpriteBox, boundary);

	Vec2D offset;
	AARectangle mapBoundary;

	for (size_t i = 0; i < m_CollisionBoundaries.size(); i++)
	{
		// Move the boundaries
		m_CollisionBoundaries[i].MoveBy(delta);

		// Get their individual map boundaries
		mapBoundary = boundary;
		GetBoundaryOffset(offset, i);
		mapBoundary.MoveBy(offset);

		WrapAroundBoundary(&m_CollisionBoundaries[i], mapBoundary);
	}
}

void Asteroid::GetBoundaryOffset(Vec2D& offset, int boundaryID)
{
	Vec2D sWidth = Vec2D((float)App::Singleton().Width(), 0.0f);
	Vec2D sHeight = Vec2D(0.0f, (float)App::Singleton().Height());

	switch (boundaryID)
	{
	case 0:
		offset = Vec2D::Zero;
		break;
	case 1:
		offset = -sWidth;
		break;
	case 2:
		offset = sWidth;
		break;
	case 3:
		offset = -sHeight;
		break;
	case 4:
		offset = sHeight;
		break;
	}
}

void Asteroid::Hit(const bool split)
{
	m_Destroyed = true;
	m_Reproduce = split;

}

std::string Asteroid::GetSpriteName()
{
	std::string spriteName;

	switch (m_Size)
	{
		case AsteroidSize::SMALL:
		{
			spriteName = "small_rock";
			m_Score = 100;
			return spriteName;
		}
		case AsteroidSize::MEDIUM:
		{
			spriteName = "medium_rock";
			m_Score = 50;
			return spriteName;
		}
		case AsteroidSize::LARGE:
		{
			spriteName = "medium_rock2";
			m_Score = 20;
			return spriteName;
		}
		case AsteroidSize::EXTRALARGE:
		{
			spriteName = "big_rock";
			m_Score = 10;
			return spriteName;
		}
	}

	return spriteName;
}


void Asteroid::WrapAroundBoundary(Circle* colBoundary, const AARectangle& boundary)
{
	Vec2D centerPoint = colBoundary->GetCenterPoint();
	Vec2D position = centerPoint;

	if (centerPoint.GetX() < boundary.GetTopLeft().GetX())
	{
		position += Vec2D(boundary.GetWidth(), 0);
	}
	if (centerPoint.GetX() >= boundary.GetBottomRight().GetX())
	{
		position -= Vec2D(boundary.GetWidth(), 0);
	}
	if (centerPoint.GetY() < boundary.GetTopLeft().GetY())
	{
		position += Vec2D(0, boundary.GetHeight());
	}
	if (centerPoint.GetY() >= boundary.GetBottomRight().GetY())
	{
		position -= Vec2D(0, boundary.GetHeight());
	}

	colBoundary->MoveTo(position);
}

void Asteroid::WrapAroundBoundary(AARectangle* box, const AARectangle& boundary)
{
	Vec2D topLeft = box->GetTopLeft();
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

	box->MoveTo(position);
}
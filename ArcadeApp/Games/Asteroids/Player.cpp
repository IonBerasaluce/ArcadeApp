#include "Player.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"

const float Player::PLAYER_ACCELERATION = 0.005f;
const float Player::MAX_SPEED = 0.1;

Player::Player(): m_CurrentSpeed(0.0f), m_CurrentVelocity(Vec2D::Zero)
{
}

void Player::Init(Vec2D playerPos)
{
	m_bTriangle = Triangle(playerPos, PLAYER_WIDTH, PLAYER_HEIGHT);
}

void Player::Draw(Screen& screen)
{
	screen.Draw(m_bTriangle, Colour::White());

	if (m_Misiles.size() > 0)
	{
		for (auto& misile : m_Misiles)
		{
			misile.Draw(screen);
		}
	}
}

void Player::Update(uint32_t dt, const AARectangle& boundary)
{
	// Update the position according to the speed
	m_bTriangle.MoveBy(m_CurrentVelocity * dt);

	std::vector<int> misileToDelete;

	if (m_Misiles.size() > 0) 
	{
		for (size_t i = 0; i < m_Misiles.size(); i++)
		{
			// Update the misile and check if it should exist
			m_Misiles[i].Update(dt, boundary);
		}

	}

	auto i = remove_if(m_Misiles.begin(), m_Misiles.end(), [&](Misile misile) { return !boundary.ContainsPoint(misile.GetMisileLocation()); });
	if (i != m_Misiles.end())
	{
		m_Misiles.erase(i);
	}

	// Wrap the player around the screen
	WrapAroundBoundary(boundary);
}

void Player::Accelerate(uint32_t dt)
{
	Vec2D direction = GetCurrentDirection();

	Vec2D acceleration = direction * PLAYER_ACCELERATION * dt;

	Vec2D finalVelocity = m_CurrentVelocity + acceleration;

	float finalSpeed = finalVelocity.Mag();

	if (finalSpeed > MAX_SPEED)
	{
		finalSpeed = MAX_SPEED;
		finalVelocity = finalSpeed * direction;
	}

	m_bTriangle.MoveBy((m_CurrentVelocity + finalVelocity) / 2 * dt);

	m_CurrentVelocity = finalVelocity;
}

void Player::Rotate(RotationDirection rotationDirection)
{
	Vec2D direction = GetCurrentDirection();
	
	float rotationAngle = static_cast<float>(rotationDirection) * 0.3f;
	m_bTriangle.Rotate(rotationAngle);
}

void Player::ShootMissile()
{
 	if (m_Misiles.size() < 3)
	{
		// When player presses the action button we fire a misile
		Vec2D spawnLocation = m_bTriangle.GetCenterPoint() + GetCurrentDirection() * 2;
		Misile misile; 
		misile.Init(GetCurrentDirection(), spawnLocation);
		m_Misiles.push_back(misile);
	}
}

void Player::WrapAroundBoundary(const AARectangle& boundary)
{
	Vec2D centrePoint = m_bTriangle.GetCenterPoint();
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

	m_bTriangle.MoveTo(position);
}

Vec2D Player::GetCurrentDirection() const
{
	Vec2D direction = m_bTriangle.GetP0() - m_bTriangle.GetCenterPoint();
	return direction.GetUnitVec();
}
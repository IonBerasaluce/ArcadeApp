#include "Player.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"
#include "App/App.h"

#include <iostream>

const float Player::PLAYER_ACCELERATION = 0.005f;
const float Player::MAX_SPEED = 0.05f;

Player::Player(): m_Lives(3), m_IsDying(false)
{
}

void Player::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Colour& spriteColour)
{
	Vec2D startPosition = Vec2D((float)(App::Singleton().Width() / 2), (float)(App::Singleton().Height() / 2));
	AsteroidsActor::Init(spriteSheet, animationsPath, startPosition, 0.0f, spriteColour);
	m_CollisionBoundary = Circle(Vec2D::Zero, m_Sprite.GetBoundingBox().GetHeight() / 2);
	m_Score.PlayerName = "ABC";
	Reset();
}

void Player::Update(uint32_t dt, const AARectangle& boundary)
{
	if (m_IsDying)
	{
		AsteroidsActor::Update(dt);
		m_IsDying = !IsFinishedAnimation();
		return;
	}

	AsteroidsActor::Update(dt);
	WrapAroundBoundary(boundary);
	m_CollisionBoundary.MoveTo(m_Sprite.GetBoundingBox().GetCenterPoint());
}

void Player::Accelerate(uint32_t dt)
{
	//SetAnimation("thrusters", false);
	Vec2D currentVelocity = m_MovementDirection * m_Speed;

	Vec2D acceleration = m_LookingDirection * PLAYER_ACCELERATION * (float)dt;
	Vec2D finalVelocity = currentVelocity + acceleration;

	m_Speed = finalVelocity.Mag();
	SetMovementDirection(finalVelocity.GetUnitVec());

	if (m_Speed > MAX_SPEED)
	{
		m_Speed = MAX_SPEED;
	}
}

void Player::Rotate(RotationDirection rotationDirection)
{
	float rotationAngle = static_cast<float>(rotationDirection) * 0.3f;
	AsteroidsActor::Rotate(rotationAngle);
}

void Player::MoveTo(const Vec2D& position)
{
	Actor::MoveTo(position);
	m_CollisionBoundary.MoveTo(position);
}

void Player::AddToScore(uint32_t score)
{
	m_Score.score += score;
}

void Player::Reset()
{
	ResetToFirstAnimation();
	ResetPosition();
	ResetScore();
	m_Lives = 3;
	m_Speed = 0;
}

void Player::CrashedIntoAsteroid()
{
	SetAnimation("explosion", false);
	m_IsDying = true;
	LooseLife();
}

void Player::ResetScore()
{
	m_Score.score = 0;
}

void Player::ResetPosition()
{
	m_MovementDirection = Vec2D(0, -1);
	m_LookingDirection = Vec2D(0, -1);
	
	Vec2D startPosition = Vec2D((float)(App::Singleton().Width() / 2), (float)(App::Singleton().Height() / 2));
	MoveTo(startPosition);
}

void Player::ResetToFirstAnimation()
{
	SetAnimation("ship", true);
}
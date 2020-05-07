#include "Player.h"
#include "Graphics/Screen.h"
#include "Shapes/AARectangle.h"
#include "App/App.h"

const float Player::PLAYER_ACCELERATION = 0.005f;
const float Player::MAX_SPEED = 0.1;
const Vec2D Player::STARTING_POSITION = Vec2D(App::Singleton().Width() / 2, App::Singleton().Height() / 2);

Player::Player(): m_Lives(3), m_Score(0)
{
}

void Player::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Colour& spriteColour)
{
	Vec2D position = Vec2D(App::Singleton().Width() / 2, App::Singleton().Height() / 2);
	AsteroidsActor::Init(spriteSheet, animationsPath, position, 0.0f, spriteColour);
	Reset();
}

void Player::Update(uint32_t dt, const AARectangle& boundary)
{
	AsteroidsActor::Update(dt);
	WrapAroundBoundary(boundary);
}

void Player::Accelerate(uint32_t dt)
{
	SetAnimation("thusters", false);

	Vec2D direction = GetMovementDirection();
	Vec2D currentVelocity = direction * m_Speed;

	Vec2D acceleration = direction * PLAYER_ACCELERATION * dt;
	Vec2D finalVelocity = currentVelocity + acceleration;

	m_Speed = finalVelocity.Mag();
	m_MovementDirection = finalVelocity.GetUnitVec();

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

void Player::Reset()
{
	//MoveTo(STARTING_POSITION);
	ResetScore();
	m_MovementDirection = Vec2D(0, -1);
	m_Speed = 0;
	ResetToFirstAnimation();
}

void Player::ResetScore()
{
	m_Score = 0;
}

void Player::ResetToFirstAnimation()
{
	SetAnimation("ship", true);
}
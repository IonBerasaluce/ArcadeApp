#include "PacmanPlayer.h"
#include "Utils/Utils.h"

void PacmanPlayer::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Colour& spriteColour)
{
	PacmanActor::Init(spriteSheet, animationsPath, initialPos, movementSpeed, false, spriteColour );

	ResetToFirstAnimation();
	ResetScore();
	m_IsDying = false;
	ResetGhostEatenMultiplier();
}

void PacmanPlayer::Update(uint32_t dt)
{
	if (m_IsDying)
	{
		m_Sprite.Update(dt);
		m_IsDying = !IsFinishedAnimation();
		return;
	}
	
	PacmanActor::Update(dt);
}

void PacmanPlayer::SetMovementDirection(PacmanMovement direction)
{
	PacmanMovement currentDirection = GetMovementDirection();

	if (direction == PACMAN_MOVE_LEFT && currentDirection != PACMAN_MOVE_LEFT)
	{
		SetAnimation("move_left", true);
		ResetDelta();
	}
	else if(direction == PACMAN_MOVE_RIGHT && currentDirection != PACMAN_MOVE_RIGHT)
	{
		SetAnimation("move_right", true);
		ResetDelta();
	}
	else if (direction == PACMAN_MOVE_DOWN && currentDirection != PACMAN_MOVE_DOWN)
	{
		SetAnimation("move_down", true);
		ResetDelta();
	}
	else if (direction == PACMAN_MOVE_UP && currentDirection != PACMAN_MOVE_UP)
	{
		SetAnimation("move_up", true);
		ResetDelta();
	}

	PacmanActor::SetMovementDirection(direction);
}

void PacmanPlayer::ResetToFirstAnimation()
{
	SetAnimation("move_left", true);
	Stop();
}

void PacmanPlayer::EatenByGhost()
{
	SetAnimation("death", false);
	m_IsDying = true;
	ResetGhostEatenMultiplier();
}

void PacmanPlayer::ResetScore()
{
	m_Score = 0;
}

void PacmanPlayer::AteItem(uint32_t value)
{
	AddToScore(value);
}

void PacmanPlayer::AteGhost(uint32_t value)
{
	AddToScore(value * m_GhostMultiplier);
	m_GhostMultiplier *= 2;
}

void PacmanPlayer::AddToScore(uint32_t value)
{
	m_Score += value;
}

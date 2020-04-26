#include "Ghost.h"

Ghost::Ghost():m_Points(0), m_InitialPos(Vec2D::Zero), m_IsReleased(false), m_Delegate(nullptr)
{
}

void Ghost::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Colour& spriteColour)
{
	Actor::Init(spriteSheet, animationsPath, initialPos, movementSpeed, false, spriteColour);
	m_InitialPos = initialPos;
	m_Points = NUM_POINTS_FOR_GHOST;
	m_VulnerabilityTimer = VULNERABILITY_TIME;
	ResetToFirstPosition();
}

void Ghost::Update(uint32_t dt)
{
	Vec2D position = Position();

	Actor::Update(dt);

	m_CanChangeDirection = position != Position();

	if (IsVulnerable())
	{
		m_VulnerabilityTimer += dt;
		if (m_State == GHOST_STATE_VULERABLE && m_VulnerabilityTimer >= VULNERABILITY_TIME)
		{
			SetGhostState(GHOST_STATE_VULNERABLE_ENDING);
		}

		if (m_State == GHOST_STATE_VULNERABLE_ENDING && m_VulnerabilityTimer >= VULNERABILITY_ENDING_TIME)
		{
			SetGhostState(GHOST_STATE_ALIVE);
		}
	}
}

void Ghost::SetStateToVulnerable()
{
	if (m_State != GHOST_STATE_DEAD && !IsVulnerable())
	{
		SetGhostState(GHOST_STATE_VULERABLE);
	}
}

void Ghost::SetMovementDirection(PacmanMovement direction)
{
	Actor::SetMovementDirection(direction);
	PacmanMovement movementDir = GetMovementDirection();

	if (m_State == GHOST_STATE_ALIVE)
	{
		if (movementDir == PACMAN_MOVE_RIGHT)
		{
			SetAnimation("ghost_movement_right", true);
		}
		else if (movementDir == PACMAN_MOVE_LEFT)
		{
			SetAnimation("ghost_movement_left", true);
		}
		else if (movementDir == PACMAN_MOVE_UP)
		{
			SetAnimation("ghost_movement_up", true);
		}
		else if (movementDir == PACMAN_MOVE_DOWN)
		{
			SetAnimation("ghost_movement_down", true);
		}
	}
	else if(m_State == GHOST_STATE_DEAD)
	{
		if (movementDir == PACMAN_MOVE_RIGHT)
		{
			SetAnimation("ghost_dead_right", true);
		}
		else if (movementDir == PACMAN_MOVE_LEFT)
		{
			SetAnimation("ghost_dead_left", true);
		}
		else if (movementDir == PACMAN_MOVE_UP)
		{
			SetAnimation("ghost_dead_up", true);
		}
		else if (movementDir == PACMAN_MOVE_DOWN)
		{
			SetAnimation("ghost_dead_down", true);
		}
	}
}

void Ghost::Stop()
{
	SetMovementDirection(PacmanMovement::PACMAN_MOVE_NONE);
}

void Ghost::EatenByPacman()
{
	SetGhostState(GHOST_STATE_DEAD);
}

void Ghost::ResetToFirstPosition()
{
	m_Sprite.SetPosition(m_InitialPos);
	SetMovementDirection(PacmanMovement::PACMAN_MOVE_NONE);
	m_VulnerabilityTimer = 0;
	SetGhostState(GhostState::GHOST_STATE_ALIVE);
	m_CanChangeDirection = true;
	m_IsReleased = false;

	if (m_Delegate)
	{
		m_Delegate->GhostWasResetToFirstposition();
	}
}

void Ghost::ReleasedFromPen()
{
	m_IsReleased = true;
	if (m_Delegate)
	{
		m_Delegate->GhostWasReleasedFromPen();
	}
}

void Ghost::SetGhostState(GhostState state)
{
	if (m_Delegate)
	{
		m_Delegate->GhostDelegateGhostStateChangedTo(m_State, state);
	}

	m_State = state;

	switch (m_State)
	{
		case GHOST_STATE_ALIVE:
		{
			SetMovementDirection(GetMovementDirection());
			SetMovementSpeed(GHOST_MOVEMENT_SPEED);
			break;
		}
		case GHOST_STATE_VULERABLE:
		{
			m_Sprite.SetAnimation("ghost_vulnerable", true);
			m_VulnerabilityTimer = 0;
			SetMovementSpeed(GHOST_VULNERABLE_MOVEMENT_SPEED);
			break;
		}
		case GHOST_STATE_VULNERABLE_ENDING:
		{
			m_Sprite.SetAnimation("ghost_vulnerable_ending", true);
			m_VulnerabilityTimer = 0;
			break;
		}
		case GHOST_STATE_DEAD:
		{
			SetMovementDirection(GetMovementDirection());
			SetMovementSpeed(GHOST_BACK_TO_PEN_SPEED);
			break;
		}
	}
}

#include <assert.h>
#include <algorithm>

#include "GhostAI.h"
#include "PacmanPlayer.h"
#include "Graphics/Screen.h"
#include "PacmanLevel.h"
#include "Shapes/Circle.h"

GhostAI::GhostAI()
{}

void GhostAI::Init(Ghost& ghost, uint32_t lookAheadDistance, const Vec2D scatterTarget, const Vec2D& ghostPenTarget, const Vec2D& ghostExitPenPosition, GhostName name)
{
	m_GhostPenTarget = ghostPenTarget;
	m_GhostExitPenPosition = ghostExitPenPosition;
	m_ScatterTarget = scatterTarget;
	m_LookaheadDistance = lookAheadDistance;
	m_ptrGhost = &ghost;
	n_Name = name;
	std::random_device r;
	m_AIRandomGenerator.seed(r());

	m_Timer = 0;
	SetState(GHOST_AI_STATE_SCATTER);
	m_LastState = GhostAIState::GHOST_AI_STATE_SCATTER;

}

PacmanMovement GhostAI::Update(uint32_t dt, const PacmanPlayer& pacman, const PacmanLevel& level, const std::vector <Ghost>& ghosts)
{
	if (m_ptrGhost)
	{

		if (m_State == GhostAIState::GHOST_AI_STATE_START)
		{
			return PacmanMovement::PACMAN_MOVE_NONE;
		}

		if (m_State == GhostAIState::GHOST_AI_STATE_IN_PEN)
		{
			m_Timer += dt;
			if (m_Timer >= GhostAI::PEN_WAIT_DURATION)
			{
				SetState(GhostAIState::GHOST_AI_STATE_EXIT_PEN);
			}

			return PacmanMovement::PACMAN_MOVE_NONE;
		}

		// Check when you get to the pen where the ghost shoudl stop moving
		if (m_State == GhostAIState::GHOST_AI_STATE_GO_TO_PEN && m_ptrGhost->Position() == m_GhostPenTarget)
		{
			SetState(GhostAIState::GHOST_AI_STATE_IN_PEN);
			m_ptrGhost->SetGhostState(GhostState::GHOST_STATE_ALIVE);
			return PacmanMovement::PACMAN_MOVE_NONE;
		}

		// Ghost is ready to leave the pen so set to scatter
		if (m_State == GhostAIState::GHOST_AI_STATE_EXIT_PEN && m_ptrGhost->Position() == m_GhostExitPenPosition)
		{
			SetState(GhostAIState::GHOST_AI_STATE_SCATTER);
		}
		
		if (m_State == GhostAIState::GHOST_AI_STATE_SCATTER)
		{
			m_Timer += dt;
			if (m_Timer >= GhostAI::SCATTER_MODE_DURATION)
			{
				SetState(GhostAIState::GHOST_AI_STATE_CHASE);
			}
		}

		PacmanMovement currentDirection = m_ptrGhost->GetMovementDirection();

		std::vector<PacmanMovement> tempDirections;
		std::vector<PacmanMovement> possibleDirections;

		possibleDirections = GetPerpendicularMovements(currentDirection);

		if (currentDirection != PacmanMovement::PACMAN_MOVE_NONE)
		{
			possibleDirections.push_back(currentDirection);
		}

		for (const auto& pDirection : possibleDirections)
		{
			tempDirections.push_back(pDirection);
		}

		possibleDirections.clear();

		for (const auto& direction : tempDirections)
		{
			if (!level.WillCollide(*m_ptrGhost, *this, direction))
			{
				possibleDirections.push_back(direction);
			}
		}

		assert(possibleDirections.size() >= 1 && "Why can't we move?");
		std::sort(possibleDirections.begin(), possibleDirections.end(), [](const PacmanMovement& direction1, const PacmanMovement& direction2) 
		{
			return direction1 < direction2;
		});

		if (m_ptrGhost->IsVulnerable() && (m_State == GhostAIState::GHOST_AI_STATE_SCATTER || m_State == GhostAIState::GHOST_AI_STATE_CHASE))
		{
			// When vulnerable, the ghost move randomly
			std::uniform_int_distribution<size_t> distribution(0, possibleDirections.size() - 1);
			return possibleDirections[static_cast<int>(distribution(m_AIRandomGenerator))];
		}

		if (m_State == GhostAIState::GHOST_AI_STATE_CHASE)
		{
			ChangeTarget(GetChaseTarget(dt, pacman, level, ghosts));
		}

		PacmanMovement directionToGoIn = PacmanMovement::PACMAN_MOVE_NONE;

		uint32_t lowestDistance = UINT32_MAX;

		// Which direction to take
		for (const auto& direction : possibleDirections)
		{
			Vec2D movementVec = GetMovementVector(direction) * m_LookaheadDistance;
			AARectangle bbox = m_ptrGhost->GetBoundingBox();

			bbox.MoveBy(movementVec);

			uint32_t distanceToTarget = bbox.GetCenterPoint().Distance(m_Target);

			if (distanceToTarget < lowestDistance)
			{
				directionToGoIn = direction;
				lowestDistance = distanceToTarget;
			}
		}
		
		assert(directionToGoIn != PacmanMovement::PACMAN_MOVE_NONE);

		return directionToGoIn;
	}
	return PacmanMovement::PACMAN_MOVE_NONE;
}

void GhostAI::Draw(Screen& screen)
{
	if (m_ptrGhost)
	{
		Circle targetCircle = Circle(m_Target, 4);

		screen.Draw(targetCircle, m_ptrGhost->GetSpriteColour(), true, m_ptrGhost->GetSpriteColour());

		AARectangle bbox = m_ptrGhost->GetBoundingBox();
		bbox.MoveBy(GetMovementVector(m_ptrGhost->GetMovementDirection()) * m_ptrGhost->GetBoundingBox().GetWidth());
		Colour c = Colour(m_ptrGhost->GetSpriteColour().GetRed(), m_ptrGhost->GetSpriteColour().GetGreen(), m_ptrGhost->GetSpriteColour().GetBlue(), 200);

		screen.Draw(bbox, m_ptrGhost->GetSpriteColour(), true, c);
	}
}

void GhostAI::GhostDelegateGhostStateChangedTo(GhostState lastState, GhostState state)
{
	if (m_ptrGhost && m_ptrGhost->IsReleased() && (lastState == GhostState::GHOST_STATE_VULERABLE || lastState == GhostState::GHOST_STATE_VULNERABLE_ENDING) && !(IsInPen() || WantsToLeavePen()))
	{
		m_ptrGhost->SetMovementDirection(GetOppositeDirection(m_ptrGhost->GetMovementDirection()));
	}

	if (state == GhostState::GHOST_STATE_DEAD)
	{
		SetState(GhostAIState::GHOST_AI_STATE_GO_TO_PEN);
	}
}

void GhostAI::GhostWasReleasedFromPen()
{
	if (m_State == GhostAIState::GHOST_AI_STATE_START)
	{
		SetState(GhostAIState::GHOST_AI_STATE_EXIT_PEN);
	}
}

void GhostAI::GhostWasResetToFirstposition()
{
	SetState(GhostAIState::GHOST_AI_STATE_START);
}

void GhostAI::SetState(GhostAIState state)
{
	if (m_State == GhostAIState::GHOST_AI_STATE_SCATTER || m_State == GhostAIState::GHOST_AI_STATE_CHASE)
	{
		m_LastState = m_State;
	}

	m_State = state;

	switch (state)
	{
	case GhostAIState::GHOST_AI_STATE_IN_PEN:
		m_Timer = 0;
		break;
	case GhostAIState::GHOST_AI_STATE_GO_TO_PEN:
		break;
	case GhostAIState::GHOST_AI_STATE_EXIT_PEN:
	{
		Vec2D target = { m_GhostPenTarget.GetX() + m_ptrGhost->GetBoundingBox().GetWidth() / 2, m_GhostPenTarget.GetY() - m_ptrGhost->GetBoundingBox().GetHeight() / 2 };
		ChangeTarget(target);
		break;
	}
	case GhostAIState::GHOST_AI_STATE_SCATTER:
		m_Timer = 0;
		ChangeTarget(m_ScatterTarget);
		break;
	default:
		break;
	}
}

Vec2D GhostAI::GetChaseTarget(uint32_t dt, PacmanPlayer pacman, const PacmanLevel& level, std::vector<Ghost> ghosts)
{
	Vec2D target;

	switch (n_Name)
	{
		case BLINKY:
		{
			// Follow pacman
			target = pacman.GetBoundingBox().GetCenterPoint();
		}
		break;
		case PINKY:
		{
			// Targets 2 blocks ahead of pacman to try to ambush pacman
			target = pacman.GetBoundingBox().GetCenterPoint() + 2 * GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth();
		}
			break;
		case INKY:
		{
			// Specfic offset from Blinky's position causing illusion of coordination between ghosts
			Vec2D pacmanOffsetPoint = pacman.GetBoundingBox().GetCenterPoint() + (GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth());

			target = (pacmanOffsetPoint - ghosts[BLINKY].GetBoundingBox().GetCenterPoint()) * 2 + ghosts[BLINKY].GetBoundingBox().GetCenterPoint();
		}
			break;
		case CLYDE:
		{
			// If Clyde is close to pacman he moves back to his Scatter place
			auto distanceToPacman = m_ptrGhost->GetBoundingBox().GetCenterPoint().Distance(pacman.GetBoundingBox().GetCenterPoint());
			if (distanceToPacman > pacman.GetBoundingBox().GetWidth() * 4)
			{
				target = pacman.GetBoundingBox().GetCenterPoint();
			}
			else
			{
				target = m_ScatterTarget;
			}
		}
		break;
		case NUM_GHOSTS:
			assert(false && "SHOULD NOT BE PASSING NUM_GHOSTS AS THE GHOST NAME!");
		}

	return target;
}

#pragma once

#include "Ghost.h"
#include "PacmanGameUtils.h"
#include <random>
#include <stdint.h>

class PacmanLevel;
class PacmanPlayer;
class Screen;
class Pacman;

enum GhostAIState
{
	GHOST_AI_STATE_START = 0,
	GHOST_AI_STATE_IN_PEN,
	GHOST_AI_STATE_CHASE,
	GHOST_AI_STATE_SCATTER,
	GHOST_AI_GO_TO_PEN,
	GHOST_AI_EXIT_PEN
};

class GhostAI
{
public:
	GhostAI();
	void Init(Ghost& ghost, uint32_t lookAheadDistance, const Vec2D scatterTarget, const Vec2D& ghostPenTarget, const Vec2D& ghostExitPenPosition, GhostName name);

	PacmanMovement Update(uint32_t dt, const PacmanPlayer& pacman, const PacmanLevel& level, const std::vector <Ghost>& ghosts);
	void Draw(Screen& screen);

	inline bool WantsToLeavePen() const { return m_State == GHOST_AI_EXIT_PEN; }
	inline bool IsInPen() const { return m_State == GhostAIState::GHOST_AI_STATE_IN_PEN || GhostAIState::GHOST_AI_STATE_START;}
	inline bool IsEnteringPen() const { return m_State == GhostAIState::GHOST_AI_GO_TO_PEN; }

private:

	static const uint32_t PEN_WAIT_DURATION = 5000;
	static const uint32_t SCATTER_MODE_DURATION = 10000;

	void SetState(GhostAIState state);
	inline void ChangeTarget(const Vec2D& target) { m_Target = target; };
	Vec2D GetChaseTarget(uint32_t dt, PacmanPlayer pacman, const PacmanLevel& level, std::vector<Ghost> ghosts);

	Vec2D m_GhostExitPenPosition;
	Vec2D m_GhostPenTarget;

	Vec2D m_ScatterTarget;
	Vec2D m_Target;
	uint32_t m_LookaheadDistance;
	Ghost* m_ptrGhost;
	GhostName n_Name;
	std::default_random_engine m_AIRandomGenerator;
	GhostAIState m_State;
	GhostAIState m_LastState;

	uint64_t m_Timer;
};
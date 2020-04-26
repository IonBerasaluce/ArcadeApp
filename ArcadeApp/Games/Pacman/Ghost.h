#pragma once

#include "Actor.h"
#include "Graphics/Colour.h"
#include "PacmanGameUtils.h"
#include "Shapes/AARectangle.h"

enum GhostName
{
	BLINKY = 0,
	PINKY,
	INKY,
	CLYDE,
	NUM_GHOSTS
};

enum GhostState
{
	GHOST_STATE_ALIVE = 0,
	GHOST_STATE_VULERABLE,
	GHOST_STATE_VULNERABLE_ENDING,
	GHOST_STATE_DEAD
};

// Pure virtual function and the ghost will have one of these delegates ( the AI )
class GhostDelegate
{
public:
	virtual ~GhostDelegate() {}
	virtual void GhostDelegateGhostStateChangedTo(GhostState lastState, GhostState state) = 0;
	virtual void GhostWasReleasedFromPen() = 0;
	virtual void GhostWasResetToFirstposition() = 0;
};

class Ghost : public Actor
{
public:
	static const uint32_t VULNERABILITY_TIME = 6000;
	static const uint32_t VULNERABILITY_ENDING_TIME = 4000;
	static const uint32_t NUM_POINTS_FOR_GHOST = 200;

	Ghost();
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Colour& spriteColour = Colour::White()) override;

	virtual void Update(uint32_t dt) override;
	void SetStateToVulnerable();
	virtual void SetMovementDirection(PacmanMovement direction) override;
	virtual void Stop() override;

	void EatenByPacman();
	void ResetToFirstPosition();

	inline bool IsDead() const { return m_State == GHOST_STATE_DEAD; }
	inline bool IsVulnerable() const { return m_State == GHOST_STATE_VULERABLE || m_State == GHOST_STATE_VULNERABLE_ENDING; }
	inline bool  IsAlive() const { return m_State == GHOST_STATE_ALIVE; }
	inline uint32_t GetPoints() const { return m_Points; }
	inline void LockCanChangeDirection() { m_CanChangeDirection = false; }
	inline bool CanChangeDirection() const { return m_CanChangeDirection; }
	inline bool IsReleased() const { return m_IsReleased; }

	inline void SetGhostDelegate(GhostDelegate& ghostDelegate) { m_Delegate = &ghostDelegate; }
	void ReleasedFromPen();

private:
	void SetGhostState(GhostState state);

	friend class GhostAI;

	uint32_t m_VulnerabilityTimer;
	uint32_t m_Points;
	GhostState m_State;
	bool m_CanChangeDirection;
	bool m_IsReleased;
	Vec2D m_InitialPos;

	GhostDelegate* m_Delegate;
};
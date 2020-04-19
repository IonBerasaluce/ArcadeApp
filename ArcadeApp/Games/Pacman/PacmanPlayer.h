#pragma once
#include "Actor.h"

class Screen;
class SpriteSheet;

class PacmanPlayer : public Actor
{
public:
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Colour& spriteColour = Colour::White());
	
	virtual void Update(uint32_t dt) override;

	virtual void SetMovementDirection(PacmanMovement direction) override;

	void ResetToFirstAnimation();
	void EatenByGhost();
	void ResetScore();
	void AddItem(uint32_t value);
	void AteGhost(uint32_t value);

	inline void ResetGhostEatenMultiplier() { m_GhostMultiplier = 1; }
	inline uint32_t Score() const { return m_Score; }

private:

	void AddToScore(uint32_t value);

	uint32_t m_Score;
	uint32_t m_GhostMultiplier;
	bool m_IsDying;
};
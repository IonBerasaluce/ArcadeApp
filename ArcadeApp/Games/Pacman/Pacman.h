#pragma once
#include "Games/Game.h"
#include "PacmanLevel.h"
#include "PacmanPlayer.h"
#include "Graphics/SpriteSheet.h"
#include "Inputs/InputAction.h"
#include "Ghost.h"
#include "GhostAI.h"


enum PacmanGameState
{
	LEVEL_STARTING = 0,
	PLAY_GAME,
	LOST_LIFE,
	GAME_OVER
};


class Pacman: public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

	void DrawLives(Screen& screen);
	void SetUpGhosts();

private:
	void ResetGame();
	void ResetPacman();

	void UpdatePacmanMovement();
	void HandleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction);
	
	std::vector<Ghost> m_Ghosts;
	std::vector<GhostAI> m_GhostAI;

	PacmanLevel m_Level;
	PacmanMovement m_PressedDirection;
	SpriteSheet m_PacmanSpriteSheet;
	PacmanPlayer m_Pacman;

	size_t m_NumLives;
	uint32_t m_ReleaseGhostTimer;

	PacmanGameState m_GameState;
	uint32_t m_LevelStartingTimer;

	AARectangle m_StringRect;
};
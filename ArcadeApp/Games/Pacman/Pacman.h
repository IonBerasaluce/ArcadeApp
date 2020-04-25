#pragma once
#include "Games/Game.h"
#include "PacmanLevel.h"
#include "PacmanPlayer.h"
#include "Graphics/SpriteSheet.h"
#include "Inputs/InputAction.h"
#include "Ghost.h"


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

	PacmanLevel m_Level;
	PacmanMovement m_PressedDirection;
	SpriteSheet m_PacmanSpriteSheet;
	PacmanPlayer m_Pacman;

	size_t m_NumLives;
	
};
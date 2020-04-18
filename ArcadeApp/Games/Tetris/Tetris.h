#pragma once
#include "Games/Game.h"
#include "Tetromino.h"
#include "TetrisLevel.h"

enum class TetrisGameState
{
	IN_GAME_OVER = 0, 
	IN_SERVE, 
	IN_PLAY
};

class Tetris : public Game
{

public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;

	void ResetGame();

	virtual const std::string& GetName() const override;

private:

	TetrisGameState m_GameState;
	Tetromino m_Block;
	Tetromino m_NextBlock;
	TetrisLevel m_TetrisLevel;
	int m_BlocksAccumulated;
	
	AARectangle m_LevelBoundary;
	uint32_t m_TimeAccumulated = 0;

	Vec2D m_PieceStartPosition;
};
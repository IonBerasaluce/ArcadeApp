#pragma once
#include "Games/Game.h"
#include "App/App.h"
#include "Paddle.h"
#include "Ball.h"
#include "LevelBoundary.h"
#include "BreakoutGameLevel.h"
#include "App/HighScoreTable.h"
#include "Scenes/HighScoreScene.h"

#include <vector>


enum BreakoutGameState
{
	IN_PLAY = 0,
	IN_SERVE, 
	IN_GAME_OVER
};

class BreakOut : public Game
{
public:
	
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

private:
	const int NUM_LIVES = 3;

	void ResetGame(size_t toLevel=0);

	BreakoutGameLevel& GetCurrentLevel() { return m_Levels[m_CurrentLevel]; }
	void SetToServeState();

	bool IsBallPassCutOffY() const;
	void ReduceLifeByOne();

	bool IsGameOver() const { return m_Lives < 0; }

	const float INITIAL_BALL_SPEED = 100.0f;
	const Vec2D INITIAL_BALL_VEL = Vec2D(100, 100);
	Paddle m_Paddle;
	Ball m_Ball;
	LevelBoundary m_levelBoundary;

	std::vector<BreakoutGameLevel> m_Levels;
	size_t m_CurrentLevel;
	const BitmapFont& m_Font = App::Singleton().GetFont();

	BreakoutGameState m_GameState;
	HighScoreTable m_HighScoreTable;
	
	ScoreInformation  m_Score;

	int m_Lives;
	float m_YCutOff;
};
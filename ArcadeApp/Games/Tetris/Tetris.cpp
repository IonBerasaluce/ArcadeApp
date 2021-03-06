#include <thread>
#include <chrono>
#include <stdlib.h> 

#include "Tetris.h"
#include "TetrisLevel.h"
#include "Inputs/GameController.h"
#include "Graphics/Screen.h"
#include "App/App.h"
#include "Scenes/HighScoreScene.h"


void Tetris::Init(GameController& controller)
{
	controller.ClearAll();
	
	ResetGame();

	ButtonAction moveLeft;
	moveLeft.key = GameController::LeftKey();
	moveLeft.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == TetrisGameState::IN_PLAY)
		{
			if (GameController::IsPressed(state))
			{
				m_Block.SetXMovementDirection(XBlockDirection::LEFT);
			}
		}
	};

	controller.AddInputActionForKey(moveLeft);

	ButtonAction moveRight;
	moveRight.key = GameController::RightKey();
	moveRight.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == TetrisGameState::IN_PLAY)
		{
			if (GameController::IsPressed(state))
			{
				m_Block.SetXMovementDirection(XBlockDirection::RIGHT);
			}
		}
	};

	controller.AddInputActionForKey(moveRight);

	ButtonAction moveDown;
	moveDown.key = GameController::DownKey();
	moveDown.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == TetrisGameState::IN_PLAY)
		{
			if (GameController::IsPressed(state))
			{
				m_Block.SetYMovementDirection(YBlockDirection::DOWN);
			}
		}
	};

	controller.AddInputActionForKey(moveDown);

	// This is temporary - we will modify a rotation intentions field which then in Update we check collision and kicks
	ButtonAction rotateClockwise;
	rotateClockwise.key = GameController::XKey();
	rotateClockwise.action = [this](uint32_t, InputState state)
	{
		if (m_GameState == TetrisGameState::IN_PLAY)
		{
			if (GameController::IsPressed(state))
			{
				m_Block.SetPieceRotation(3);
			}
		}
	};
	
	controller.AddInputActionForKey(rotateClockwise);

	ButtonAction rotateAntiClockwise;
	rotateAntiClockwise.key = GameController::ZKey();
	rotateAntiClockwise.action = [this](uint32_t, InputState state)
	{
		if (m_GameState == TetrisGameState::IN_PLAY)
		{
			if (GameController::IsPressed(state))
			{
				m_Block.SetPieceRotation(1);
			}
		}
	};

	controller.AddInputActionForKey(rotateAntiClockwise);

	ButtonAction serveAction;
	serveAction.key = GameController::ActionKey();
	serveAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			if (m_GameState == TetrisGameState::IN_SERVE)
			{
				m_GameState = TetrisGameState::IN_PLAY;
			}
		}
	};

	controller.AddInputActionForKey(serveAction);

	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == TetrisGameState::IN_SERVE || m_GameState == TetrisGameState::IN_GAME_OVER)
		{
			if (GameController::IsPressed(state))
			{
				// Go back to the previous scene
				App::Singleton().PopScene();
			}
		}
	};

	controller.AddInputActionForKey(backAction);
}

void Tetris::Update(uint32_t dt)
{

	if(m_GameState == TetrisGameState::IN_PLAY)
	{
		m_TimeAccumulated += dt;

		if (m_TetrisLevel.DoesPieceFit(m_Block, m_Block.GetPositionChange(), m_Block.GetRotationChange()))
		{
			m_Block.Update(dt);
		}

		// Check if we have to increase the level
		if ((m_TetrisLevel.GetCompletedLines() % 2 <= m_CompletedLines % 2) && (m_CompletedLines < m_TetrisLevel.GetCompletedLines()))
		{
			m_Level -= 50;
		}

		m_CompletedLines = m_TetrisLevel.GetCompletedLines();

		if (m_TimeAccumulated > m_Level)
		{
			// Move the block down if we can do so
			Vec2D dy = { 0.0f, 1.0f * (float)Tetromino::BLOCK_HEIGHT };
			if (m_TetrisLevel.DoesPieceFit(m_Block, dy, 0))
			{
				m_Block.SetXMovementDirection(XBlockDirection::STOP);
				m_Block.MoveBy(dy);
				m_Block.Update(dt);
			}
			else
			{
				// Add piece to the field
				m_TetrisLevel.AddPiece(m_Block);

				// If there are completed lines update the field - clear the line
				m_TetrisLevel.Update(dt, m_Score.score);

				// m_Block becomes m_nextBlock and starts to move down if it can and add it 
				m_Block.Init(m_NextBlock.GetPieceType(), m_LevelBoundary, m_PieceStartPosition);

				// m_NextBlock gets generated
				Vec2D nextPiecePosition = { m_LevelBoundary.GetBottomRight().GetX() + 35, m_LevelBoundary.GetTopLeft().GetY() };
				m_NextBlock.Init(static_cast<TetrominoType>(rand() % 7), m_LevelBoundary, nextPiecePosition);

				if (!m_TetrisLevel.DoesPieceFit(m_Block, Vec2D(0, 0), 0))
				{
					// Game Over
					m_GameState = TetrisGameState::IN_GAME_OVER;

					// Update the highScore table
					m_HighScoreTable.UpdateTable(m_Score);
					m_HighScoreTable.SaveToFile();

					// Make the highScoreScene Before resetting the game
					std::unique_ptr<HighScoreScene> highScoreScene = std::make_unique<HighScoreScene>(m_HighScoreTable);

					// Push the game over highscore scene
					App::Singleton().PushScene(std::move(highScoreScene));
				}
			}

			m_TimeAccumulated = 0;
		}
	}

	if (m_GameState == TetrisGameState::IN_GAME_OVER)
	{
		// Reset the Game
		ResetGame();
	}
}

void Tetris::Draw(Screen& screen)
{  
	m_Block.Draw(screen);
	m_NextBlock.Draw(screen);
	
	// Draw the level
	m_TetrisLevel.Draw(screen, m_Level, m_Score.score);
}


void Tetris::ResetGame()
{
	m_TetrisLevel.Init(Vec2D(Tetromino::BLOCK_WIDTH, Tetromino::BLOCK_HEIGHT));

	m_CompletedLines = 0;
	
	m_LevelBoundary = m_TetrisLevel.GetLevelBoundary();

	m_PieceStartPosition = { m_LevelBoundary.GetTopLeft().GetX() + Tetromino::BLOCK_WIDTH * 3, m_LevelBoundary.GetTopLeft().GetY() };

	Vec2D nextPiecePosition = { m_LevelBoundary.GetBottomRight().GetX() + 20, m_LevelBoundary.GetTopLeft().GetY()};
	
	m_Block.Init(static_cast<TetrominoType>(rand() % 7), m_LevelBoundary, m_PieceStartPosition);
	m_NextBlock.Init(static_cast<TetrominoType>(rand() % 7), m_LevelBoundary, nextPiecePosition);

	m_HighScoreTable.Init("Tetris");
	m_Score.PlayerName = "ABC";
	m_Score.score = 0;

	m_GameState = TetrisGameState::IN_SERVE;
	
	m_Level = 500;
	m_CompletedLines = 0;
}

const std::string& Tetris::GetName() const
{
	static std::string name = "Tetris";
	return name;
}

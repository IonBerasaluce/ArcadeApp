#include <iostream>

#include "BreakOut.h"
#include "Inputs/GameController.h"
#include "Shapes/Circle.h"
#include "Graphics/BitmapFont.h"

/*
Paddle
	Move side to side (by the user)
	Stops at the edges of the sceen
	Width and Height
	Bounces the ball
		Bounces the ball differently based on the position that was hit on the paddle
		
		---======---
		+-----------+
		|			|
		+-----------+

Ball
	Bounces off the walls and the paddle
	Width and height
	Velocity
	Position
	
Bricks
	Gets placed on specific spots
	Have "health" 
	Colour 
	Position
	Width and Height

Level
	Container for the game 
	Loads level from the levels.txt file
	Handle collisions?
	Contain the walls of the screens
	Reset the level

Game
	Contains all of the objects (Paddle, Ball, Levels)
	Player which has three lives
	Score
	States: Serve, inGame Game Over
	Rest the game
*/



void BreakOut::Init(GameController& controller)
{
	controller.ClearAll();

	ResetGame();

	ButtonAction serveAction;
	serveAction.key = GameController::ActionKey();
	serveAction.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == IN_SERVE)
		{
			if (GameController::IsPressed(state))
			{
				m_GameState = IN_PLAY;

				if (m_Paddle.IsMovingLeft())
				{
					m_Ball.SetVelocity(Vec2D(-INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				}
				else
				{
					m_Ball.SetVelocity(Vec2D(INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				}
			}
		}
		else if (m_GameState == IN_GAME_OVER)
		{
			if (GameController::IsPressed(state))
			{
				ResetGame();
			}
		}
	};

	controller.AddInputActionForKey(serveAction);

	ButtonAction leftKeyAction;
	leftKeyAction.key = GameController::LeftKey();
	leftKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == IN_PLAY || m_GameState == IN_SERVE)
		{
			if (GameController::IsPressed(state))
			{
				m_Paddle.SetMovementDirection(PaddleDirection::LEFT);
			}
			else
			{
				m_Paddle.UnSetMovementDirection(PaddleDirection::LEFT);
			}
		}
	};

	controller.AddInputActionForKey(leftKeyAction);

	ButtonAction rightKeyAction;
	rightKeyAction.key = GameController::RightKey();
	rightKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == IN_PLAY || m_GameState == IN_SERVE)
		{
			if (GameController::IsPressed(state))
			{
				m_Paddle.SetMovementDirection(PaddleDirection::RIGHT);
			}
			else
			{
				m_Paddle.UnSetMovementDirection(PaddleDirection::RIGHT);
			}
		}
	};

	controller.AddInputActionForKey(rightKeyAction);

	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == IN_SERVE || m_GameState == IN_GAME_OVER)
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

void BreakOut::Update(uint32_t dt)
{
	if (m_GameState == IN_SERVE)
	{
		m_Paddle.Update(dt, m_Ball);
		SetToServeState();
	}

	if (m_GameState == IN_PLAY)
	{
		m_Ball.Update(dt);
		m_Paddle.Update(dt, m_Ball);

		BoundaryEdge edge;

		if (m_Paddle.Bounce(m_Ball))
		{
			return;
		}

		if (m_levelBoundary.HasCollided(m_Ball, edge))
		{
			m_Ball.Bounce(edge);
			return;
		}

		GetCurrentLevel().Update(dt, m_Ball, m_Score);

		if (IsBallPassCutOffY())
		{
			ReduceLifeByOne();
			if (!IsGameOver())
			{
				SetToServeState();
			}
			else
			{
				m_GameState = IN_GAME_OVER;

				m_HighScoreTable.UpdateTable(m_Score);
				m_HighScoreTable.SaveToFile();
				
				std::unique_ptr<HighScoreScene> highScoreScene = std::make_unique<HighScoreScene>(m_HighScoreTable);
				m_HighScoreTable.ClearTable();

				ResetGame(m_CurrentLevel);

				App::Singleton().PushScene(std::move(highScoreScene));
			}
		}
		else if (GetCurrentLevel().IsLevelComplete())
		{
			m_CurrentLevel = (m_CurrentLevel + 1) % m_Levels.size();
			ResetGame(m_CurrentLevel);
		}
	}
}

void BreakOut::Draw(Screen& screen)
{
	m_Ball.Draw(screen);
	m_Paddle.Draw(screen);
	GetCurrentLevel().Draw(screen);

	screen.Draw(m_levelBoundary.GetAARectangle(), Colour::White());

	Circle lifeCircle = { Vec2D(7, App::Singleton().Height() - 10), 5 };

	Line2D cutoff = { Vec2D(0, m_YCutOff), Vec2D(App::Singleton().Width(), m_YCutOff) };
	screen.Draw(cutoff, Colour::White());

	for (size_t i = 0; i < m_Lives; i++)
	{
		screen.Draw(lifeCircle, Colour::Red(), true, Colour::Red());
		lifeCircle.MoveBy(Vec2D(17, 0));
	}

	// Update the score on the screen
	std::string text = "Score " + std::to_string(m_Score.score);
	AARectangle rect = { Vec2D(App::Singleton().Width() - m_Font.GetSizeOf(text).width - 5, App::Singleton().Height() - m_Font.GetSizeOf(text).height - 6), m_Font.GetSizeOf(text).width, m_Font.GetSizeOf(text).height };
	Vec2D textDrawPosition = m_Font.GetDrawPosition(text, rect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);

	screen.Draw(m_Font, text, textDrawPosition, Colour::White());
}

const std::string& BreakOut::GetName() const
{
	static std::string name = "Break Out!";
	return name;
}

void BreakOut::ResetGame(size_t toLevel)
{
	m_Levels = BreakoutGameLevel::LoadLevelsFromFile(App::GetBasePath() + "Assets\\BreakoutLevels.txt");
		
	m_Score.PlayerName = "ABC";
	m_Score.score = 0;
	m_HighScoreTable.Init("BreakOut");

	m_Lives = NUM_LIVES;
	m_YCutOff = App::Singleton().Height() - 2 * m_Paddle.PADDLE_HEIGHT;

	AARectangle paddleRect = {Vec2D(App::Singleton().Width() / 2 - Paddle::PADDLE_WIDTH / 2, App::Singleton().Height() - 3 * Paddle::PADDLE_HEIGHT), Paddle::PADDLE_WIDTH, Paddle::PADDLE_HEIGHT};

	AARectangle levelBoundary = { Vec2D::Zero, App::Singleton().Width(), App::Singleton().Height() };
	
	m_levelBoundary = { levelBoundary };

	m_Paddle.Init(paddleRect, levelBoundary);
	m_Ball.MoveTo(Vec2D(App::Singleton().Width()/2.0f, App::Singleton().Height() * 0.75f));

	SetToServeState();
}

void BreakOut::SetToServeState()
{
	m_GameState = IN_SERVE;
	m_Ball.Stop();

	m_Ball.MoveTo(Vec2D(m_Paddle.GetAARectangle().GetCenterPoint().GetX(), m_Paddle.GetAARectangle().GetTopLeft().GetY() - m_Ball.GetRadius()));
}

bool BreakOut::IsBallPassCutOffY() const
{
	return m_Ball.GetPosition().GetY() > m_YCutOff;
}

void BreakOut::ReduceLifeByOne()
{
	if (m_Lives >= 0)
	{
		--m_Lives;
	}
}

#include <stdlib.h>
#include <time.h>

#include "AsteroidsGame.h"
#include "Graphics/Screen.h"
#include "Shapes/Triangle.h"
#include "App/App.h"

#include "Inputs/GameController.h"

namespace
{
	const uint32_t LEVEL_STARTING_TIME = 3000;
}

void AsteroidsGame::Init(GameController& controller)
{
	// Init the player
	m_AsteroidsSpriteSheet.Load("AsteroidsSprites");
	m_AnimationsPath = App::Singleton().GetBasePath() + "Assets\\AsteroidsAnimations.txt";
	m_Player.Init(m_AsteroidsSpriteSheet, m_AnimationsPath);

	// Set up the game controller
	ButtonAction accelerateAction;
	accelerateAction.key = GameController::UpKey();
	accelerateAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			m_Player.Accelerate(dt);
		}
	};

	controller.AddInputActionForKey(accelerateAction);

	ButtonAction rotateLeftAction;
	rotateLeftAction.key = GameController::LeftKey();
	rotateLeftAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			m_Player.Rotate(RotationDirection::ROTATE_LEFT);
		}
	};

	controller.AddInputActionForKey(rotateLeftAction);

	ButtonAction rotateRightAction;
	rotateRightAction.key = GameController::RightKey();
	rotateRightAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			m_Player.Rotate(RotationDirection::ROTATE_RIGHT);
		}
	};

	controller.AddInputActionForKey(rotateRightAction);

	ButtonAction fireAction;
	fireAction.key = GameController::SpaceKey();
	fireAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			ShootMissile(m_Player.Position(), m_Player.GetLookingDirection());
		}
	};

	controller.AddInputActionForKey(fireAction);

	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == AsteroidsGameState::GAME_OVER && GameController::IsPressed(state))
		{
			// Go back to the starting scene
			App::Singleton().PopScene();
		}
	};

	controller.AddInputActionForKey(backAction);

	m_Font = App::Singleton().GetFont();
	ResetGame();
}

void AsteroidsGame::Update(uint32_t dt)
{
	if (m_GameState == AsteroidsGameState::LEVEL_STARTING)
	{
		m_LevelStartingTimer += dt;
		if (m_LevelStartingTimer >= LEVEL_STARTING_TIME)
		{
			m_LevelStartingTimer = 0;
			m_GameState = AsteroidsGameState::PLAY_GAME;
		}

		if (m_LevelStartingTimer % 1000 == 0)
		{
			--m_CountDown;
		}
	}
	else if (m_GameState == AsteroidsGameState::PLAY_GAME)
	{
		m_Player.Update(dt, m_MapBoundary);
		UpdateMisiles(dt);
		UpdateAsteroids(dt);
		CalculateCollisions(m_Player);
	}
	else if (m_GameState == AsteroidsGameState::LOSS_LIFE)
	{
		// Let the misiles and the player finish their dying animations
		m_Player.Update(dt, m_MapBoundary);
		UpdateMisiles(dt);

		bool finishedMisileAnimation = true;
		for (const auto& misile : m_Misiles)
		{
			finishedMisileAnimation *= misile.IsFinishedAnimation();
		}

		if (m_Player.IsFinishedAnimation() && finishedMisileAnimation)
		{
			if (m_Player.GetLives() > 0)
			{
				ResetPlayer();
			}
			else
			{
				// Update the high score table
				m_HighScoreTable.UpdateTable(m_Player.GetScore());
				m_HighScoreTable.SaveToFile();
				m_GameState = AsteroidsGameState::GAME_OVER;
			}
		}
	}
}

void AsteroidsGame::UpdateMisiles(uint32_t dt)
{
	if (m_Misiles.size() > 0)
	{
		for (size_t i = 0; i < m_Misiles.size(); i++)
		{
			// Update the misile and check if it should exist
			m_Misiles[i].Update(dt, m_MapBoundary);
		}
	}

	auto i = remove_if(m_Misiles.begin(), m_Misiles.end(), [&](Misile misile) { return !m_MapBoundary.ContainsPoint(misile.Position()); });
	if (i != m_Misiles.end())
	{
		m_Misiles.erase(i);
	}
}

void AsteroidsGame::UpdateAsteroids(uint32_t dt)
{
	size_t size = m_Asteroids.size();
	
	// Generate 2 new asteroids when a big one is destroyed
	for (size_t i = 0; i < size; i++)
	{
		if (m_Asteroids[i].IsDestroyed() && m_Asteroids[i].GetSize() > 0 && m_Asteroids[i].Reproduce())
		{
			Asteroid currentAsteroid = m_Asteroids[i];
			GenerateAsteroids(2, currentAsteroid.Position(), static_cast<AsteroidSize>(currentAsteroid.GetSize() - 1));
		}
	}

	// Delete the destroyed asteroids from the map
	auto k = remove_if(m_Asteroids.begin(), m_Asteroids.end(), [&](Asteroid asteroid) { return asteroid.IsDestroyed(); });
	if (k != m_Asteroids.end())
	{
		m_Asteroids.erase(k);
	}

	// If all asteroids are destroyed we add new asteroids
	if (m_Asteroids.size() > 0)
	{
		// Update the position of the remaining asteroid
		for (auto& asteroid : m_Asteroids)
		{
			asteroid.Update(dt, m_MapBoundary);
		}
	}
	else
	{
		GenerateAsteroids(3);
	}

	// Generate new asteroids as time passes
	m_Accumulator += dt;
	if (m_Accumulator >= 1000)
	{
		if (m_Asteroids.size() < 15)
		{
			GenerateAsteroids(1);
			m_Accumulator = 0;
		}
	}
}

void AsteroidsGame::Draw(Screen& screen)
{

	if (m_Asteroids.size() > 0)
	{
		for (auto& asteroid : m_Asteroids)
		{
			asteroid.Draw(screen);
		}
	}

	if (m_GameState == AsteroidsGameState::PLAY_GAME || m_GameState == AsteroidsGameState::LOSS_LIFE)
	{
		m_Player.Draw(screen);

		if (m_Misiles.size() > 0)
		{
			for (auto& misile : m_Misiles)
			{
				misile.Draw(screen);
			}
		}
	}
	else
	{	
		uint32_t sWidth = App::Singleton().Width();
		uint32_t sHeight = App::Singleton().Height();

		Vec2D textPos;
		AARectangle rect = AARectangle(Vec2D((float)sWidth / 2, (float)sHeight / 2) - Vec2D(3, 3), 6, 6);
		
		if (m_GameState == AsteroidsGameState::LEVEL_STARTING)
		{
			if (m_CountDown > 0)
			{
				textPos = m_Font.GetDrawPosition(std::to_string(m_CountDown), rect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);
				screen.Draw(m_Font, std::to_string(m_CountDown), textPos, Colour::Red());
			}
		}
		else
		{
			textPos = m_Font.GetDrawPosition("Game Over!", rect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);
			screen.Draw(m_Font, "Game Over!", textPos, Colour::Red());
		}
	}
}


void AsteroidsGame::CalculateCollisions(Player& player)
{
	for (size_t i = 0; i < m_Asteroids.size(); i++)
	{
		// Collision Asteroid to player
		if (m_Asteroids[i].GetCollisionBox().Intersect(m_Player.GetCollisionBox()))
		{
			m_Asteroids[i].Hit(false);
			m_Player.CrashedIntoAsteroid();
			m_GameState = AsteroidsGameState::LOSS_LIFE;
		}

		// Collision Asteroid to missile
		for (size_t j = 0; j < m_Misiles.size(); j++)
		{
			if (!m_Misiles[j].IsHit())
			{
				if (m_Asteroids[i].GetCollisionBox().Intersect(m_Misiles[j].GetCollisionBox()))
				{
					m_Misiles[j].Hit(m_Asteroids[i].GetSize() == 0);
					m_Asteroids[i].Hit();
					m_Player.AddToScore(m_Asteroids[i].GetScore());
				}
			}
		}
	}
}

void AsteroidsGame::GenerateAsteroids(const int n, const Vec2D& position, AsteroidSize size)
{
	uint32_t sWidth = App::Singleton().Width();
	uint32_t sHeight = App::Singleton().Height();
	float x, y;
	Vec2D newPosition;
	int newSize;
	srand(time(NULL));

	int corridor = 40;

	// This should generate both the split asteroids and the new asteroids in the surrounding map
	for (size_t i = 0; i < n; i++)
	{
		if (size == AsteroidSize::EXTRALARGE)
		{
			++m_NumAsteroids;
		}

		if (position == Vec2D::Zero)
		{
			// We are placing the asteroid in the surronding area of the map (i.e not in the center nor where the player is)
			float y = (float)(rand() % sHeight);
			x = 0.0f;
			
			if (y > corridor || y < sHeight - corridor)
			{
				if ((rand() % 2) == 1)
				{
					x = (float)(rand() % corridor);
				}
				else
				{
					x = (float)(sWidth - (rand() % corridor));
				}
			}
			else
			{
				x = (float)(rand() % sWidth);
			}

			newPosition = Vec2D(x, y);
		}
		else
		{
			newPosition = position;
		}

		if (size == AsteroidSize::NONE)
		{
			// We choose a random size 
			newSize = rand() % 4;
		}
		else 
		{
			newSize = static_cast<int>(size);
		}
		
		Vec2D randomDir((float)(rand() % 10), (float)(rand() % 10));

		Asteroid asteroid;
		asteroid.Init(m_AsteroidsSpriteSheet, randomDir.GetUnitVec(), newPosition, static_cast<AsteroidSize>(newSize));
		m_Asteroids.push_back(asteroid);
	}
}

void AsteroidsGame::ShootMissile(const Vec2D& position, const Vec2D& direction)
{
	if (m_Misiles.size() < 3)
	{
		// When player presses the action button we fire a misile
		Misile misile;
		misile.Init(m_AsteroidsSpriteSheet, m_AnimationsPath, m_Player.GetLookingDirection(), m_Player.Position());
		m_Misiles.push_back(misile);
	}
}

void AsteroidsGame::ResetGame()
{
	m_MapBoundary = { Vec2D::Zero, Vec2D((float)(App::Singleton().Width()), (float)(App::Singleton().Height())) };
	m_Player.Reset();
	m_Accumulator = 0;
	m_Misiles.clear();
	ResetAsteroids();
	m_CountDown = 3;
	m_GameState = AsteroidsGameState::LEVEL_STARTING;

	m_HighScoreTable.Init("Asteroids");
}

void AsteroidsGame::ResetPlayer()
{
	m_Player.ResetPosition();
	m_Player.ResetToFirstAnimation();
	ResetAsteroids();
	m_CountDown = 3;
	m_GameState = AsteroidsGameState::LEVEL_STARTING;
}

void AsteroidsGame::ResetAsteroids()
{
	m_Asteroids.clear();
	GenerateAsteroids(3);
}

const std::string& AsteroidsGame::GetName() const
{
	static const std::string name = "Asteroids!";
	return name;
}
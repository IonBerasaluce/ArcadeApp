#include <stdlib.h>
#include <time.h>

#include "AsteroidsGame.h"
#include "Graphics/Screen.h"
#include "Shapes/Triangle.h"
#include "App/App.h"

#include "Inputs/GameController.h"

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

	ResetGame();
}

void AsteroidsGame::Update(uint32_t dt)
{
	CalculateCollisions(m_Player);

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
	
	m_Player.Update(dt, m_MapBoundary);

	// Update the misiles
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

	size_t size = m_Asteroids.size();

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

}

void AsteroidsGame::Draw(Screen& screen)
{
	m_Player.Draw(screen);

	if (m_Misiles.size() > 0)
	{
		for (auto& misile : m_Misiles)
		{
			misile.Draw(screen);
		}
	}

	if (m_Asteroids.size() > 0)
	{
		for (auto& asteroid : m_Asteroids)
		{
			asteroid.Draw(screen);
		}
	}
}


void AsteroidsGame::CalculateCollisions(Player& player)
{
	for (size_t i = 0; i < m_Asteroids.size(); i++)
	{
		if (m_Asteroids[i].GetCollisionBox().Intersect(m_Player.GetCollisionBox()))
		{
			m_Asteroids[i].Hit(false);
			m_Player.CrashedIntoAsteroid();
		}

		// Collision Asteroid to missile
		for (size_t j = 0; j < m_Misiles.size(); j++)
		{
			if (!m_Misiles[j].IsHit())
			{
				float distance = m_Asteroids[i].Position().Distance(m_Misiles[j].Position());
				if (distance < m_Asteroids[i].GetRadious())
				{
					m_Misiles[j].Hit(m_Asteroids[i].GetSize() == 0);
					m_Asteroids[i].Hit();
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
	m_Asteroids.clear();
	m_Misiles.clear();
	m_Accumulator = 0;
	GenerateAsteroids(3);
}

const std::string& AsteroidsGame::GetName() const
{
	static const std::string name = "Asteroids!";
	return name;
}
#include <stdlib.h>

#include "AsteroidsGame.h"
#include "Graphics/Screen.h"
#include "Shapes/Triangle.h"
#include "App/App.h"

#include "Inputs/GameController.h"


void AsteroidsGame::Init(GameController& controller)
{
	// Init the player
	m_AsteroidsSpriteSheet.Load("AsteroidsSprites");
	std::string animationsPath = App::Singleton().GetBasePath() + "Assets\\AsteroidsAnimations.txt";
	m_Player.Init(m_AsteroidsSpriteSheet, animationsPath);

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
			ShootMissile(m_Player.Position(), m_Player.GetMovementDirection());
		}
	};

	controller.AddInputActionForKey(fireAction);

	ResetGame();
}

void AsteroidsGame::Update(uint32_t dt)
{
	CalculateCollisions(m_Player);
	
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

	auto i = remove_if(m_Misiles.begin(), m_Misiles.end(), [&](Misile misile) { return !m_MapBoundary.ContainsPoint(misile.GetPosition()); });
	if (i != m_Misiles.end())
	{
		m_Misiles.erase(i);
	}

	int size = m_Asteroids.size();

	Vec2D randomDirection;

	for (size_t i = 0; i < size; i++)
	{
		if (m_Asteroids[i].IsDestroyed() && m_Asteroids[i].GetSize() > 1)
		{
			Asteroid currentAsteroid = m_Asteroids[i];
			GenerateAsteroids(2, currentAsteroid.GetSize() - 1, currentAsteroid.GetPosition());
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
		GenerateAsteroids(m_NumAsteroids + 1, 3);
		m_NumAsteroids += 1;
	}

}

void AsteroidsGame::Draw(Screen& screen)
{
	m_Player.Draw(screen);

	if (m_Misiles.size() > 0)
	{
		for (auto& misile : m_Misiles)
		{
			if (!misile.IsHit())
			{
				misile.Draw(screen);
			}
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
		// Collision Asteroid to player
		float distance = m_Asteroids[i].GetPosition().Distance(m_Player.Position());
		if (distance < m_Asteroids[i].GetRadious())
		{
			m_Player.LossLife();
			// TODO: Clean up here
			m_Player.Reset();
		}

		// Collision Asteroid to missile
		for (size_t j = 0; j < m_Misiles.size(); j++)
		{
			if (!m_Misiles[j].IsHit())
			{
				float distance = m_Asteroids[i].GetPosition().Distance(m_Misiles[j].GetPosition());
				if (distance < m_Asteroids[i].GetRadious())
				{
					m_Misiles[j].Hit();
					m_Asteroids[i].Hit();
				}
			}
		}
	}
}

void AsteroidsGame::GenerateAsteroids(const int n, const int size, const Vec2D& position)
{
	Vec2D newPosition;
	Vec2D randomDir;

	for (size_t i = 0; i < n; i++)
	{
		if (position == Vec2D::Zero)
		{
			newPosition = Vec2D(rand() % App::Singleton().Width(), rand() % App::Singleton().Height());
		}
		else
		{
			newPosition = position;
		}

		randomDir = Vec2D(rand(), rand());

		Asteroid asteroid;
		asteroid.Init(newPosition, randomDir, size);
		m_Asteroids.push_back(asteroid);
	}
}

void AsteroidsGame::ShootMissile(const Vec2D& position, const Vec2D& direction)
{
	if (m_Misiles.size() < 3)
	{
		// When player presses the action button we fire a misile
		Vec2D spawnLocation = position + direction * 2;
		Misile misile;
		misile.Init(direction, spawnLocation);
		m_Misiles.push_back(misile);
	}
}


void AsteroidsGame::ResetGame()
{
	// When creating the boundary we make it slightly bigger than the screen to improve the wrap effect
	m_MapBoundary = { Vec2D(-10, -10), Vec2D(App::Singleton().Width() + 10, App::Singleton().Height() + 10) };

	m_Player.Reset();
	
	m_Asteroids.clear();
	m_Misiles.clear();

	Asteroid asteroid;
	asteroid.Init(m_Player.Position() + Vec2D(40, 0), Vec2D(2,-4), 3);
	m_Asteroids.push_back(asteroid);

	m_NumAsteroids = 2;
}

const std::string& AsteroidsGame::GetName() const
{
	static const std::string name = "Asteroids!";
	return name;
}
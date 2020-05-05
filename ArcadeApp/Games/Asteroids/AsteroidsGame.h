#pragma once

#include "Games/Game.h"
#include "Player.h"
#include "Misile.h"
#include "Shapes/AARectangle.h"
#include "Asteroid.h"
#include "App/App.h"

/*
Level
	- Handle score
	- Spawn asteroids in the correct places
	- Handle collisions
	- Delete teh bullets of the number of bullets in the screen exceeds 3
	- Make sure the wrapping around the map is smooth

Player
	- Shoot bullets
	- Moves around the map in any direction a wraps around all edges
	- Obeis laws of inertia
	- Handle the animations

Asteroids
	- 3 sizes
	- When an asteroid is hit the score increases and the asteroid splits into 2 smaller asteroids
	- They must spawn in any location but not on the player
	- Obey the laws of inertia
	- Wrap around the edges of the map

Bullets
	- Max of 3 in the screen
	- move really fast, in the direction of the palyer, once the player shoots the bullets.
	- If the bullet collides with the asteroid then the asteroid blows up.

Start Scene

*/

class AsteroidsGame : public Game
{
public:
	virtual void Init(GameController& controller);
	virtual void Update(uint32_t dt);
	virtual void Draw(Screen& screen);
	virtual const std::string& GetName() const;

	void CalculateCollisions(Player& player);

	void GenerateAsteroids(const int n, const int size, const Vec2D& position = Vec2D::Zero);

private:
	void ShootMissile(const Vec2D& position, const Vec2D& direction);
	void ResetGame();

private:
	Player m_Player;
	AARectangle m_MapBoundary;
	std::vector<Asteroid> m_Asteroids;
	std::vector<Misile> m_Misiles;
	int m_NumAsteroids;
};
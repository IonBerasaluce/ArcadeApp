#include "Pacman.h"
#include "App/App.h"

/*
- Pacman
	- Can eat ghosts if are a super pellet
	- Eats pellets
	- Moves in 4 directions ( up, down, left, right)
	- Animates
	- Can be eaten by ghosts 
	- Lives
	- Score

-Ghost
	- Can eat Pacman if not in vulnerable state
	- Moves in 4 directions (up, down, left, right)
	- Has different states it can be in (Alive,  Vulnerable, Vulnerable-ending, dead)
	- Has AI
	- Starting point
	- Has a score value if eaten

-AI
	- Each ghost has a specific personality
		- Chase (Blinky)
		- Cut off/ Ambush (Pinky)
		- Pinser attack with blinky (Inky)
		- Wander (Clyde)
	- Scatter mode

- Level
	- Contains all the walls (maze)
	- Contins the pellets
	- Wraps Pacman/Ghosts at specific locations
	- Gate that releases the Ghosts
	- Single background image

- Pellet
	- Super pellet - turns ghosts to vulnerable state
	- Has a specific score value

- Special Items
	- Score

- UI
	- Display the score

*/


void Pacman::Init(GameController& controller)
{
	m_Level.Init(App::Singleton().GetBasePath() + "Assets\\Pacman_level.txt");
}

void Pacman::Update(uint32_t dt)
{
	m_Level.Update(dt);
}

void Pacman::Draw(Screen& screen)
{
	m_Level.Draw(screen);
}

const std::string& Pacman::GetName() const
{
	static std::string name = "Pacman!";
	return name;
}

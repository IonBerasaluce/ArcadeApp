#include "Pacman.h"
#include "App/App.h"
#include "PacmanGameUtils.h"
#include "Inputs/GameController.h"
#include "Graphics/BitMapFont.h"

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

namespace {
	const std::string SCORE_STR = "Score  ";
	const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
	const size_t MAX_NUM_LIVES = 3;
}


void Pacman::Init(GameController& controller)
{

	m_PacmanSpriteSheet.Load("PacmanSprites");
	std::string animationsPath = App::Singleton().GetBasePath() + "Assets\\Pacman_animations.txt";
	m_Pacman.Init(m_PacmanSpriteSheet, animationsPath, Vec2D::Zero, PACMAN_MOVEMENT_SPEED, false);

	m_Level.Init(App::Singleton().GetBasePath() + "Assets\\Pacman_level.txt", &m_PacmanSpriteSheet);

	ResetGame();

	//Set up the Game controller actions
	ButtonAction leftAction;
	leftAction.key = GameController::LeftKey();
	leftAction.action = [this](uint32_t dt, InputState state)
	{
		HandleGameControllerState(dt, state, PACMAN_MOVE_LEFT);
	};

	controller.AddInputActionForKey(leftAction);

	//Set up the Game controller actions
	ButtonAction rightAction;
	rightAction.key = GameController::RightKey();
	rightAction.action = [this](uint32_t dt, InputState state)
	{
		HandleGameControllerState(dt, state, PACMAN_MOVE_RIGHT);
	};

	controller.AddInputActionForKey(rightAction);

	//Set up the Game controller actions
	ButtonAction upAction;
	upAction.key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state)
	{
		HandleGameControllerState(dt, state, PACMAN_MOVE_UP);
	};

	controller.AddInputActionForKey(upAction);

	//Set up the Game controller actions
	ButtonAction downAction;
	downAction.key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state)
	{
		HandleGameControllerState(dt, state, PACMAN_MOVE_DOWN);
	};

	controller.AddInputActionForKey(downAction);

}

void Pacman::Update(uint32_t dt)
{
	UpdatePacmanMovement();
	m_Pacman.Update(dt);

	for (size_t i = 0; i < NUM_GHOSTS; i++)
	{
		m_Ghosts[i].Update(dt);
	}

	m_Level.Update(dt, m_Pacman, m_Ghosts);

	if (m_Level.IsLevelOver())
	{
		m_Level.IncreaseLevel();
		ResetPacman();
	}
}

void Pacman::Draw(Screen& screen)
{
	m_Level.Draw(screen);
	m_Pacman.Draw(screen);

	for (size_t i = 0; i < NUM_GHOSTS; i++)
	{
		m_Ghosts[i].Draw(screen);
	}

	//Draw the score
	{
		Vec2D levelOffset = m_Level.GetLayoutOffset();
		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());
		
		BitmapFont font = App::Singleton().GetFont();
		
		std::string scoreString = std::to_string(m_Pacman.Score());

		Vec2D textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreString, highScoreRect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);

		screen.Draw(font, SCORE_STR + scoreString, textDrawPosition);
	}

	DrawLives(screen);
}

const std::string& Pacman::GetName() const
{
	static std::string name = "Pacman!";
	return name;
}

void Pacman::DrawLives(Screen& screen)
{
	const uint32_t X_PAD = 1;
	Sprite sprite = m_PacmanSpriteSheet.GetSprite(PACMAN_LIFE_SPRITE_NAME);

	uint32_t xPos = X_PAD;

	for (int i = 0; i < m_NumLives; i++)
	{
		screen.Draw(m_PacmanSpriteSheet, PACMAN_LIFE_SPRITE_NAME, Vec2D(xPos, App::Singleton().Height() - sprite.height));
		xPos += X_PAD + sprite.width;
	}
}

void Pacman::SetUpGhosts()
{
	m_Ghosts.resize(NUM_GHOSTS);
	std::string path = App::Singleton().GetBasePath() + "Assets\\Ghost_animations.txt";

	Ghost blinky;
	blinky.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[BLINKY], GHOST_MOVEMENT_SPEED, true, Colour::Red());
	blinky.SetMovementDirection(PACMAN_MOVE_LEFT);
	m_Ghosts[BLINKY] = blinky;

	Ghost inky;
	inky.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[INKY], GHOST_MOVEMENT_SPEED, true, Colour::Cyan());
	inky.SetMovementDirection(PACMAN_MOVE_UP);
	m_Ghosts[INKY] = inky;

	Ghost pinky;
	pinky.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[PINKY], GHOST_MOVEMENT_SPEED, true, Colour::Pink());
	pinky.SetMovementDirection(PACMAN_MOVE_DOWN);
	m_Ghosts[PINKY] = pinky;

	Ghost clyde;
	clyde.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[CLYDE], GHOST_MOVEMENT_SPEED, true, Colour::Orange());
	clyde.SetMovementDirection(PACMAN_MOVE_UP);
	m_Ghosts[CLYDE] = clyde;
}

void Pacman::ResetGame()
{
	m_NumLives = MAX_NUM_LIVES;
	m_PressedDirection = PACMAN_MOVE_NONE;

	SetUpGhosts();
	m_Pacman.ResetScore();
	m_Level.ResetToFirstLevel();
	ResetPacman();
}

void Pacman::ResetPacman()
{
	m_Pacman.MoveTo(m_Level.GetPacmanSpawnLocation());
	m_Pacman.ResetToFirstAnimation();
}

void Pacman::UpdatePacmanMovement()
{
	if (m_PressedDirection != PACMAN_MOVE_NONE)
	{
		if (!m_Level.WillCollide(m_Pacman.GetBoundingBox(), m_PressedDirection))
		{
			m_Pacman.SetMovementDirection(m_PressedDirection);
		}
	}
}

void Pacman::HandleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction)
{
	if (GameController::IsPressed(state))
	{
		m_PressedDirection = direction;
	}
	else if (GameController::IsReleased(state) && m_PressedDirection == direction)
	{
		m_PressedDirection = PACMAN_MOVE_NONE;
	}

}

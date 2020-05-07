#include "Pacman.h"
#include "App/App.h"
#include "PacmanGameUtils.h"
#include "Inputs/GameController.h"
#include "Graphics/BitMapFont.h"
#include "App/HighScoreTable.h"

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
	const uint32_t RELEASE_GHOST_TIME = 1000;
	const std::string READY_STR = "Ready!";
	const std::string GAME_OVER_STR = "Game Over!";
	const uint32_t LEVEL_STARTING_TIME = 3000;
}


void Pacman::Init(GameController& controller)
{
	m_ReleaseGhostTimer = 0;
	m_PacmanSpriteSheet.Load("PacmanSprites");
	std::string animationsPath = App::Singleton().GetBasePath() + "Assets\\Pacman_animations.txt";
	m_Pacman.Init(m_PacmanSpriteSheet, animationsPath, Vec2D::Zero, PACMAN_MOVEMENT_SPEED, false);

	m_Level.Init(App::Singleton().GetBasePath() + "Assets\\Pacman_level.txt", &m_PacmanSpriteSheet);

	m_StringRect = AARectangle(Vec2D(0, m_Level.GetInGameTextYPos()), App::Singleton().Width(), m_Pacman.GetBoundingBox().GetHeight());


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

	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [this](uint32_t dt, InputState state)
	{
		if (m_GameState == PacmanGameState::GAME_OVER && GameController::IsPressed(state))
		{
			// Go back to the previous scene
			App::Singleton().PopScene();
		}
	};

	controller.AddInputActionForKey(backAction);
}

void Pacman::Update(uint32_t dt)
{

	if (m_GameState == PacmanGameState::LEVEL_STARTING)
	{
		m_Pacman.ResetGhostEatenMultiplier();
		m_LevelStartingTimer += dt;

		if (m_LevelStartingTimer >= LEVEL_STARTING_TIME)
		{
			m_LevelStartingTimer = 0;
			m_GameState = PacmanGameState::PLAY_GAME;
		}
	}
	else if (m_GameState == PacmanGameState::PLAY_GAME)
	{
		UpdatePacmanMovement();
		m_Pacman.Update(dt);

		m_ReleaseGhostTimer += dt;

		for (size_t i = 0; i < NUM_GHOSTS; i++)
		{
			Ghost& ghost = m_Ghosts[i];
			GhostAI& ghostAI = m_GhostAI[i];

			if (m_ReleaseGhostTimer >= RELEASE_GHOST_TIME && ghostAI.IsInPen() && !ghost.IsReleased())
			{
				m_ReleaseGhostTimer = 0;
				ghost.ReleasedFromPen();
			}

			auto direction = ghostAI.Update(dt, m_Pacman, m_Level, m_Ghosts);

			if ((ghost.IsReleased() && ghost.CanChangeDirection()) || (ghost.IsReleased() && ghostAI.WantsToLeavePen() && direction != PacmanMovement::PACMAN_MOVE_NONE))
			{
				// Now the ghosts are free to move
				if (direction != ghost.GetMovementDirection())
				{
					ghost.SetMovementDirection(direction);
					ghost.LockCanChangeDirection();
				}
			}
 			ghost.Update(dt);

			// Handle eating ghosts if they are vulnerable and pacman got close to them 
			if (ghost.IsVulnerable() && m_Pacman.GetEatingBoundingBox().Intersects(ghost.GetBoundingBox()))
			{
				ghost.EatenByPacman();
				m_Pacman.AteGhost(ghost.GetPoints());
			}
			else if (ghost.IsAlive() && ghost.GetEatingBoundingBox().Intersects(m_Pacman.GetBoundingBox()))
			{
				m_NumLives--;
				m_GameState = PacmanGameState::LOST_LIFE;
				m_Pacman.EatenByGhost();
				m_PressedDirection = PacmanMovement::PACMAN_MOVE_NONE;
				m_Pacman.SetMovementDirection(PacmanMovement::PACMAN_MOVE_NONE);
				return;
			}
		}

		m_Level.Update(dt, m_Pacman, m_Ghosts, m_GhostAI);

		if (m_Level.IsLevelOver())
		{
			m_Level.IncreaseLevel();
			ResetPacman();
		}
	}
	else if (m_GameState == PacmanGameState::LOST_LIFE)
	{
		// Update the death animation
		m_Pacman.Update(dt); 
		if (m_Pacman.IsFinishedAnimation())
		{
			if (m_NumLives > 0)
			{
				ResetPacman();
			}
			else
			{
				//temporary code here
				ScoreInformation score;
				score.score = m_Pacman.Score();
				score.PlayerName = "ABC";
				m_HighScoreTable.UpdateTable(score);
				m_HighScoreTable.SaveToFile();
				m_GameState = PacmanGameState::GAME_OVER;
			}
		}
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

	const auto& font = App::Singleton().GetFont();
	Vec2D textDrawPosition;

	//Draw the score
	{
		Vec2D levelOffset = m_Level.GetLayoutOffset();
		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());
		
		std::string scoreString = std::to_string(m_Pacman.Score());

		textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreString, highScoreRect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);

		screen.Draw(font, SCORE_STR + scoreString, textDrawPosition);
	}
	
	if (m_GameState == PacmanGameState::LEVEL_STARTING)
	{
		textDrawPosition = font.GetDrawPosition(READY_STR, m_StringRect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);

		screen.Draw(font, READY_STR, textDrawPosition, Colour::Yellow());
	}
	else if (m_GameState == PacmanGameState::GAME_OVER)
	{
		textDrawPosition = font.GetDrawPosition(GAME_OVER_STR, m_StringRect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);

		screen.Draw(font, GAME_OVER_STR, textDrawPosition, Colour::Yellow());
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
	const Vec2D BLINKY_SCATTER_POS = Vec2D(App::Singleton().Width() - 24, 0);
	const Vec2D INKY_SCATTER_POS = Vec2D(App::Singleton().Width(), App::Singleton().Height());
	const Vec2D PINKY_SCATTER_POS = Vec2D(24, 0);
	const Vec2D CLYDE_SCATTER_POS = Vec2D(0, App::Singleton().Height());

	std::string path = App::Singleton().GetBasePath() + "Assets\\Ghost_animations.txt";

	m_Ghosts.resize(NUM_GHOSTS);
	m_GhostAI.resize(NUM_GHOSTS);

	Ghost blinky;
	blinky.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[BLINKY], GHOST_MOVEMENT_SPEED, true, Colour::Red());
	blinky.SetMovementDirection(PACMAN_MOVE_LEFT);
	m_Ghosts[BLINKY] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.Init(m_Ghosts[BLINKY], blinky.GetBoundingBox().GetWidth(), BLINKY_SCATTER_POS, m_Level.GhostSpawnPoints()[PINKY], m_Level.GhostSpawnPoints()[BLINKY], BLINKY);
	m_GhostAI[BLINKY] = blinkyAI;

	Ghost pinky;
	pinky.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[PINKY], GHOST_MOVEMENT_SPEED, true, Colour::Pink());
	pinky.SetMovementDirection(PACMAN_MOVE_DOWN);
	m_Ghosts[PINKY] = pinky;

	auto pinkyAI = GhostAI();
	pinkyAI.Init(m_Ghosts[PINKY], pinky.GetBoundingBox().GetWidth(), PINKY_SCATTER_POS, m_Level.GhostSpawnPoints()[PINKY], m_Level.GhostSpawnPoints()[BLINKY], PINKY);
	m_GhostAI[PINKY] = pinkyAI;

	Ghost inky;
	inky.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[INKY], GHOST_MOVEMENT_SPEED, true, Colour::Cyan());
	inky.SetMovementDirection(PACMAN_MOVE_UP);
	m_Ghosts[INKY] = inky;

	auto inkyAI = GhostAI();
	inkyAI.Init(m_Ghosts[INKY], inky.GetBoundingBox().GetWidth(), INKY_SCATTER_POS, m_Level.GhostSpawnPoints()[PINKY], m_Level.GhostSpawnPoints()[BLINKY], INKY);
	m_GhostAI[INKY] = inkyAI;

	Ghost clyde;
	clyde.Init(m_PacmanSpriteSheet, path, m_Level.GhostSpawnPoints()[CLYDE], GHOST_MOVEMENT_SPEED, true, Colour::Orange());
	clyde.SetMovementDirection(PACMAN_MOVE_UP);
	m_Ghosts[CLYDE] = clyde;
	
	auto clydeAI = GhostAI();
	clydeAI.Init(m_Ghosts[CLYDE], clyde.GetBoundingBox().GetWidth(), CLYDE_SCATTER_POS, m_Level.GhostSpawnPoints()[PINKY], m_Level.GhostSpawnPoints()[BLINKY], CLYDE);
	m_GhostAI[CLYDE] = clydeAI;

	for (size_t i = 0; i < GhostName::NUM_GHOSTS; i++)
	{
		m_Ghosts[i].SetGhostDelegate(m_GhostAI[i]);
	}
}

void Pacman::ResetGame()
{
	m_NumLives = MAX_NUM_LIVES;
	m_PressedDirection = PACMAN_MOVE_NONE;

	// Initialise the highscore table
	m_HighScoreTable.Init("Pacman");

	SetUpGhosts();
	m_Pacman.ResetScore();
	m_Level.ResetToFirstLevel();
	ResetPacman();
}

void Pacman::ResetPacman()
{
	m_ReleaseGhostTimer = 0;
	m_GameState = LEVEL_STARTING;
	m_Pacman.MoveTo(m_Level.GetPacmanSpawnLocation());
	m_Pacman.ResetToFirstAnimation();

	for (auto& ghost : m_Ghosts)
	{
		ghost.ResetToFirstPosition();
	}

	// Blinky is laways free from the beggining 
	m_Ghosts[BLINKY].ReleasedFromPen();
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

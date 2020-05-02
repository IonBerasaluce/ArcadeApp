#include "AsteroidsGame.h"
#include "Graphics/Screen.h"
#include "Shapes/Triangle.h"
#include "App/App.h"

#include "Inputs/GameController.h"


void AsteroidsGame::Init(GameController& controller)
{
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

	m_Player.Init(Vec2D(App::Singleton().Width() / 2, App::Singleton().Height() / 2));
}

void AsteroidsGame::Update(uint32_t dt)
{
	m_Player.Update(dt);
}

void AsteroidsGame::Draw(Screen& screen)
{
	m_Player.Draw(screen);
}

const std::string& AsteroidsGame::GetName() const
{
	static const std::string name = "Asteroids!";
	return name;
}

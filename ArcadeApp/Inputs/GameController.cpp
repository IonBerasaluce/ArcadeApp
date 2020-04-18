#include "GameController.h"
#include "SDL.h"

GameController::GameController():m_MouseMovedAction(nullptr)
{
}

InputAction GameController::GetActionForKey(InputKey key)
{
	for (const auto& ButtonAction : m_ButtonActions)
	{
		if (key == ButtonAction.key)
		{
			return ButtonAction.action;
		}
	}

	return [](uint32_t, InputState) {};
}

void GameController::AddInputActionForKey(const ButtonAction& buttonAction)
{
	m_ButtonActions.push_back(buttonAction);
}

void GameController::ClearAll()
{
	m_ButtonActions.clear();
}

bool GameController::IsPressed(InputState state)
{
	return state == SDL_PRESSED;
}

bool GameController::IsReleased(InputState state)
{
	return state == SDL_RELEASED;
}

InputKey GameController::ActionKey()
{
	return static_cast<InputKey>(SDLK_RETURN);
}

InputKey GameController::CancelKey()
{
	return static_cast<InputKey>(SDLK_s);
}

InputKey GameController::LeftKey()
{
	return static_cast<InputKey>(SDLK_LEFT);
}

InputKey GameController::RightKey()
{
	return static_cast<InputKey>(SDLK_RIGHT);
}

InputKey GameController::UpKey()
{
	return static_cast<InputKey>(SDLK_UP);
}

InputKey GameController::DownKey()
{
	return static_cast<InputKey>(SDLK_DOWN);
}

InputKey GameController::ZKey()
{
	return static_cast<InputKey>(SDLK_z);
}

InputKey GameController::XKey()
{
	return static_cast<InputKey>(SDLK_x);
}

MouseInputAction GameController::GetMouseButtonActionForMouseButton(MouseButton button)
{
	for (const auto& buttonAction: m_MouseButtonActions)
	{
		if (button == buttonAction.mouseButton)
		{
			return buttonAction.mouseInputAction;
		}
	}

	return [](InputState state, const MousePosition& pos){};
}

void GameController::AddMouseButtonAction(const MouseButtonAction& mouseButtonAction)
{
	m_MouseButtonActions.push_back(mouseButtonAction);
}

MouseButton GameController::LeftMouseButton()
{
	return static_cast<MouseButton>(SDL_BUTTON_LEFT);
}

MouseButton GameController::RightMouseButton()
{
	return static_cast<MouseButton>(SDL_BUTTON_RIGHT);
}

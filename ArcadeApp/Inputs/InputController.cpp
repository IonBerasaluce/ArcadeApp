#include "SDL.h"

#include "InputController.h"
#include "GameController.h"

InputController::InputController():m_Quit(nullptr), m_noptrCurrentController(nullptr)
{

}

void InputController::Init(InputAction quitAction)
{
	m_Quit = quitAction;
}

void InputController::Update(uint32_t dt)
{
	SDL_Event sdlEvent;
	
	while(SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
		{
			m_Quit(dt, SDL_PRESSED);
		}
		case SDL_MOUSEMOTION:
		{
			if (m_noptrCurrentController)
			{
				if (MouseMovedAction mouseMoved = m_noptrCurrentController->GetMouseMovedAction())
				{
					MousePosition position;
					position.xPos = sdlEvent.motion.x;
					position.yPos = sdlEvent.motion.y;
					mouseMoved(position);
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		{
			if (m_noptrCurrentController)
			{
				MouseInputAction action = m_noptrCurrentController->GetMouseButtonActionForMouseButton(static_cast<MouseButton>(sdlEvent.button.button));
				MousePosition position;
				position.xPos = sdlEvent.button.x;
				position.yPos = sdlEvent.button.y;
				action(static_cast<InputState>(sdlEvent.key.state), position);
			}
			break;
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if (m_noptrCurrentController)
			{
				InputAction action = m_noptrCurrentController->GetActionForKey(sdlEvent.key.keysym.sym);

				action(dt, static_cast<InputState>(sdlEvent.key.state));
			}
			break;
		}
	}
}

void InputController::SetGameController(GameController* controller)
{
	m_noptrCurrentController = controller;
}

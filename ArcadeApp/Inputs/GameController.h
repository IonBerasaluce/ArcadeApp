#pragma once
#include <vector>

#include "InputAction.h"

class GameController
{
public:
	GameController();
	InputAction GetActionForKey(InputKey key);
	void AddInputActionForKey(const ButtonAction& buttonAction);
	void ClearAll();

	static bool IsPressed(InputState state);
	static bool IsReleased(InputState state);

	static InputKey ActionKey();
	static InputKey CancelKey();
	static InputKey LeftKey();
	static InputKey RightKey();
	static InputKey UpKey();
	static InputKey DownKey();

	static InputKey ZKey();
	static InputKey XKey();
	static InputKey SpaceKey();

	inline const MouseMovedAction& GetMouseMovedAction() { return m_MouseMovedAction; }
	inline void SetMouseMovedAction(const MouseMovedAction& mouseMovedAction) { m_MouseMovedAction = mouseMovedAction; }

	MouseInputAction GetMouseButtonActionForMouseButton(MouseButton button);
	void AddMouseButtonAction(const MouseButtonAction& mouseButtonAction);

	static MouseButton LeftMouseButton();
	static MouseButton RightMouseButton();
		
private:
	std::vector<ButtonAction> m_ButtonActions;
	std::vector<MouseButtonAction> m_MouseButtonActions;
	MouseMovedAction m_MouseMovedAction;

};

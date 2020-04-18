#include "NotImplementedScene.h"
#include "App/App.h"
#include "Shapes/AARectangle.h"
#include "Inputs/InputAction.h"
#include "Graphics/Screen.h"

void NotImplementedScene::Init()
{
	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			// Go back to the previous scene
			App::Singleton().PopScene();
		}
	};

	m_GameController.AddInputActionForKey(backAction);
}

void NotImplementedScene::Update(uint32_t dt)
{
}

void NotImplementedScene::Draw(Screen& theScreen)
{
	const BitmapFont& font = App::Singleton().GetFont();

	AARectangle rect = { Vec2D::Zero, App::Singleton().Width(), App::Singleton().Height()};

	Vec2D textDrawPosition;

	textDrawPosition = font.GetDrawPosition(GetSceneName(), rect, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);

	theScreen.Draw(font, GetSceneName(), textDrawPosition, Colour::Red());
}

const std::string& NotImplementedScene::GetSceneName() const
{
	static std::string name = "Not Implemented!";
	return name;
}

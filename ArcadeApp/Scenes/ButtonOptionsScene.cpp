#include "ButtonOptionsScene.h"
#include "App/App.h"
#include "Graphics/BitmapFont.h"
#include "Utils/Utils.h"

ButtonOptionsScene::ButtonOptionsScene(const std::vector<std::string>& optionNames, const Colour& textColour):
	m_HighlightedOption(0)
{
	const BitmapFont& font = App::Singleton().GetFont();

	for (size_t i = 0; i < optionNames.size(); i++)
	{
		m_Buttons.push_back(Button(font, textColour));
		m_Buttons.back().SetButtonText(optionNames[i]);
	}

	if (optionNames.size() > 0)
	{
		// Highlight the first button
		m_Buttons[m_HighlightedOption].SetHighlighted(true);
	}
}

void ButtonOptionsScene::Init()
{
	ButtonAction upAction;
	upAction.key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			SetPreviousButtonHighlighted();
		}
	};

	m_GameController.AddInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			SetNextButtonHighlighted();
		}
	};

	m_GameController.AddInputActionForKey(downAction);

	ButtonAction acceptAction;
	acceptAction.key = GameController::ActionKey();
	acceptAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			ExecuteCurrentButtonAction();
		}
	};

	m_GameController.AddInputActionForKey(acceptAction);

	uint32_t height = App::Singleton().Height();
	uint32_t width = App::Singleton().Width();

	const BitmapFont& font = App::Singleton().GetFont();

	Size fontSize = font.GetSizeOf(m_Buttons[0].GetButtonText());

	const int BUTTON_PAD = 10;
	unsigned int buttonHeight = fontSize.height + BUTTON_PAD * 2;

	uint32_t maxButtonWidth = fontSize.width;

	for (const auto& button : m_Buttons)
	{
		Size s = font.GetSizeOf(button.GetButtonText());

		if (s.width > maxButtonWidth)
		{
			maxButtonWidth = s.width;
		}
	}

	maxButtonWidth += BUTTON_PAD * 2;

	const uint32_t Y_PAD = 1;

	uint32_t yOffset = height / 2 - ((buttonHeight + Y_PAD) * static_cast<uint32_t>(m_Buttons.size())) / 2;
	
	for (auto& button : m_Buttons)
	{
		button.Init(Vec2D((float)(width / 2 - maxButtonWidth / 2), (float)yOffset), maxButtonWidth, buttonHeight);

		yOffset += buttonHeight + Y_PAD;
	}

	m_Buttons[m_HighlightedOption].SetHighlighted(true);

}

void ButtonOptionsScene::Update(uint32_t dt)
{

}

void ButtonOptionsScene::Draw(Screen& theScreen)
{
	for (auto& button : m_Buttons)
	{
		button.Draw(theScreen);
	}
}

void ButtonOptionsScene::HighlightCurrentButton()
{
	for (auto& button : m_Buttons)
	{
		button.SetHighlighted(false);
	}

	m_Buttons[m_HighlightedOption].SetHighlighted(true);
}

void ButtonOptionsScene::SetButtonActions(const std::vector<Button::ButtonAction>& buttonActions)
{
	for (size_t i = 0; i < m_Buttons.size(); i++)
	{
		m_Buttons[i].SetButtonAction(buttonActions[i]);
	}
}

void ButtonOptionsScene::SetNextButtonHighlighted()
{
	m_HighlightedOption = (m_HighlightedOption + 1) % m_Buttons.size();
	
	HighlightCurrentButton();
}

void ButtonOptionsScene::SetPreviousButtonHighlighted()
{
	--m_HighlightedOption;

	if (m_HighlightedOption < 0)
	{
		m_HighlightedOption = static_cast<int>(m_Buttons.size()) - 1;
	}

	HighlightCurrentButton();

}

void ButtonOptionsScene::ExecuteCurrentButtonAction()
{
	m_Buttons[m_HighlightedOption].ExecuteAction();
}

#pragma once

#include <string>
#include <vector>

#include "Scene.h"
#include "App/Button.h"

class Screen;

class ButtonOptionsScene : public Scene
{
public:

	ButtonOptionsScene(const std::vector<std::string>& optionNames, const Colour& textColour);

	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& theScreen) override;

	void HighlightCurrentButton();
	void SetButtonActions(const std::vector<Button::ButtonAction>& buttonActions);

private:

	void SetNextButtonHighlighted();
	void SetPreviousButtonHighlighted();
	void ExecuteCurrentButtonAction();

	std::vector<Button> m_Buttons;
	int m_HighlightedOption;
};
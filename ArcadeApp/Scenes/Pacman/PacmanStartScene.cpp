#include <memory>

#include "PacmanStartScene.h"

#include "Graphics/Colour.h"
#include "Scenes/GameScene.h"
#include "Scenes/HighScoreScene.h"
#include "Games/Pacman/Pacman.h"
#include "App/App.h"


PacmanStartScene::PacmanStartScene(): ButtonOptionsScene({ "Play Game!", "High-Scores" }, Colour::Yellow())
{
}

void PacmanStartScene::Init()
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

	std::vector<Button::ButtonAction> actions;

	actions.push_back([this]() {
		auto pacmanGame = std::make_unique<Pacman>();
		App::Singleton().PushScene(std::make_unique<GameScene>(std::move(pacmanGame)));
		});

	// Go to the high score scene
	actions.push_back([this]() {
		HighScoreTable highScoreTable;
		highScoreTable.Init("Pacman");
		std::unique_ptr<HighScoreScene> highScoreScene = std::make_unique<HighScoreScene>(highScoreTable);
		App::Singleton().PushScene(std::move(highScoreScene));
		});

	SetButtonActions(actions);

	ButtonOptionsScene::Init();
}

void PacmanStartScene::Update(uint32_t dt)
{
}

void PacmanStartScene::Draw(Screen& theScreen)
{
	ButtonOptionsScene::Draw(theScreen);
}

const std::string& PacmanStartScene::GetSceneName() const
{
	// Learn why we have to use static here for this to work
	static std::string name = "Pacman!";
	return name;
}

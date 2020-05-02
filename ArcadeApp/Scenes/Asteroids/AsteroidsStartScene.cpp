#include "AsteroidsStartScene.h"
#include "App/App.h"
#include "Inputs/InputAction.h"
#include "Scenes/HighScoreScene.h"
#include "Scenes/GameScene.h"

#include "Games/Asteroids/AsteroidsGame.h"

AsteroidsStartScene::AsteroidsStartScene():ButtonOptionsScene({ "Play Game!", "High-Scores" }, Colour::Red())
{
}

void AsteroidsStartScene::Init()
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
		auto asteroidsGame = std::make_unique<AsteroidsGame>();
		App::Singleton().PushScene(std::make_unique<GameScene>(std::move(asteroidsGame)));
		});

	// Go to the high score scene
	actions.push_back([this]() {
		HighScoreTable highScoreTable;
		highScoreTable.Init("Asteroids");
		std::unique_ptr<HighScoreScene> highScoreScene = std::make_unique<HighScoreScene>(highScoreTable);
		App::Singleton().PushScene(std::move(highScoreScene));
		});

	SetButtonActions(actions);

	ButtonOptionsScene::Init();
}

void AsteroidsStartScene::Update(uint32_t dt)
{
}

void AsteroidsStartScene::Draw(Screen& theScreen)
{
	ButtonOptionsScene::Draw(theScreen);
}

const std::string& AsteroidsStartScene::GetSceneName() const
{
	static std::string name = "Asteroids";
	return name;
}

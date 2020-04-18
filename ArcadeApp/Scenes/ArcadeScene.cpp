#include <iostream>

#include "ArcadeScene.h"
#include "Graphics/Screen.h"
#include "Graphics/Colour.h"

#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"

#include "Inputs/GameController.h"

#include "App/App.h"
#include "GameScene.h"
#include "Games/BreakOut/BreakOut.h"
#include "Games/Tetris/Tetris.h"
#include "NotImplementedScene.h"


ArcadeScene::ArcadeScene(): ButtonOptionsScene({"Tetris", "Break Out!", "Asteroids", "Pac-man"}, Colour::Cyan())
{

}

void ArcadeScene::Init()
{
	std::vector<Button::ButtonAction> actions;

	actions.push_back([this] {
		App::Singleton().PushScene(GetScene(TETRIS));
	});


	actions.push_back([this] {
		App::Singleton().PushScene(GetScene(BREAK_OUT));
	});

	actions.push_back([this] {
		App::Singleton().PushScene(GetScene(ASTEROIDS));
	});

	actions.push_back([this] {
		App::Singleton().PushScene(GetScene(PACMAN));
	});

	SetButtonActions(actions);

	ButtonOptionsScene::Init();
}

void ArcadeScene::Update(uint32_t dt)
{

}

void ArcadeScene::Draw(Screen& theScreen)
{
	ButtonOptionsScene::Draw(theScreen);
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade Scene";
	return sceneName;
}

std::unique_ptr<Scene> ArcadeScene::GetScene(eGame game)
{
	switch (game)
	{
		case TETRIS:
		{
			std::unique_ptr<Tetris> tetrisGame = std::make_unique<Tetris>();

			std::unique_ptr<GameScene> tetrisGameScene = std::make_unique<GameScene>(std::move(tetrisGame));

			return tetrisGameScene;
		}
		break;
		case BREAK_OUT:
		{
			std::unique_ptr<BreakOut> breakOutGame = std::make_unique<BreakOut>();

			std::unique_ptr<GameScene> breakOutScene = std::make_unique<GameScene>(std::move(breakOutGame));

			return breakOutScene;
		}
		break;
		case ASTEROIDS:
		{

		}
		break;
		case PACMAN:
		{

		}
		break;
	}

	std::unique_ptr<Scene> notImplementedScene = std::make_unique<NotImplementedScene>();
	
	return notImplementedScene;
}
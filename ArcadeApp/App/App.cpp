#include <memory>
#include <cassert>

#include "SDL.h"

#include "App.h"
#include "Scenes/ArcadeScene.h"
#include "Scenes/GameScene.h"


App& App::Singleton()
{
	static App theApp;
	return theApp;
}

bool App::Init(uint32_t width, uint32_t height, uint32_t mag)
{
	if (!m_Font.Load("ArcadeFont"))
	{
		std::cout << "Could not load Arcade Font!" << std::endl;
		return false;
	}

	m_noptrWindow = m_Screen.Init(width, height, mag);
	
	// Create and init our scene
	std::unique_ptr<ArcadeScene> arcadeScene = std::make_unique<ArcadeScene>();
	PushScene(std::move(arcadeScene));

	return m_noptrWindow != nullptr;
}

void App::Run()
{
	if (m_noptrWindow)
	{
		bool running = true;

		uint32_t lastTick = SDL_GetTicks();
		uint32_t currentTick = lastTick;

		uint32_t dt = 10;
		uint32_t accumulator = 0;

		m_InputController.Init([&running](uint32_t dt, InputState state) { running = false; });

		while (running)
		{
			currentTick = SDL_GetTicks();
			uint32_t frameTime = currentTick - lastTick;

			if (frameTime > 300)
			{
				frameTime = 300;
			}

			lastTick = currentTick;
			accumulator += frameTime;

			// Input
			m_InputController.Update(dt);

			Scene* topScene = App::TopScene();
			assert(topScene && "Why dont we have a scene?");

			if (topScene)
			{
				// Update
				while (accumulator >= dt)
				{
					// update the current scene by dt, this makes the game run smoothly at all frame rates
					topScene->Update(dt);
					accumulator -= dt;
				}

				// Render
				topScene->Draw(m_Screen);
			}

			m_Screen.SwapScreens();
		}
	}
}

void App::PushScene(std::unique_ptr<Scene> scene)
{
	// What the heck is this doing!! 
	// Apparently scene is an lvalue and you need to cast it as a rvalue
	assert(scene && "Don't push nullptr");
	if (scene)
	{
		scene->Init();
		m_InputController.SetGameController(scene->GetGameController());
		m_SceneStack.emplace_back(std::move(scene));
		SDL_SetWindowTitle(m_noptrWindow, TopScene()->GetSceneName().c_str());
	}
}

void App::PopScene()
{
	if (m_SceneStack.size() > 1)
	{
		m_SceneStack.pop_back();
	}

	if (TopScene())
	{
		// when we push and pop our scenes we reset our game controller
		m_InputController.SetGameController(TopScene()->GetGameController());
		SDL_SetWindowTitle(m_noptrWindow, TopScene()->GetSceneName().c_str());
	}
}

Scene* App::TopScene() //Current Scene
{
	if (m_SceneStack.empty())
	{
		return nullptr;
	}
	return m_SceneStack.back().get();
}

const std::string& App::GetBasePath()
{
	static std::string basePath = SDL_GetBasePath();

	return basePath;
}

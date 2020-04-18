#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>

#include "Graphics/Screen.h"
#include "Inputs/InputController.h"
#include "Graphics/BitmapFont.h"

struct SDL_Window;
class Scene;

class App
{
public:
	static App& Singleton();
	bool Init(uint32_t width, uint32_t height, uint32_t mag);
	void Run();

	inline uint32_t Width() const { return m_Screen.Width(); }
	inline uint32_t Height() const { return m_Screen.Height(); }

	void PushScene(std::unique_ptr<Scene> scene);
	void PopScene();
	Scene* TopScene();

	static const std::string& GetBasePath();
	inline const BitmapFont& GetFont() const { return m_Font;  }

private:
	Screen m_Screen;
	SDL_Window* m_noptrWindow;
	InputController m_InputController;

	std::vector<std::unique_ptr<Scene>> m_SceneStack;
	BitmapFont m_Font;
};
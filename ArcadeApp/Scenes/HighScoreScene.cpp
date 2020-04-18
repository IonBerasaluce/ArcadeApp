#include "HighScoreScene.h"
#include "Graphics/BitmapFont.h"
#include "Graphics/Colour.h"

HighScoreScene::HighScoreScene(HighScoreTable table): m_HighScoreTable(table)
{
}

void HighScoreScene::Init()
{
	// Add the action to restart the game
	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [](uint32_t dt, InputState state) {
		if (GameController::IsPressed(state))
		{
			// Go back to the previous scene
			App::Singleton().PopScene();
		}
	};

	m_GameController.AddInputActionForKey(backAction);

}

void HighScoreScene::Update(uint32_t dt)
{
}

void HighScoreScene::Draw(Screen& theScreen)
{
	uint32_t height = App::Singleton().Height();
	uint32_t width = App::Singleton().Width();

	const BitmapFont& font = App::Singleton().GetFont();

	Size fontSize; 
	uint32_t maxWidth = 0;
	uint32_t maxHeight = 0;

	for (const auto& score : m_HighScoreTable.GetTable())
	{
		std::string text = score.PlayerName + "   " + std::to_string(score.score);
		
		Size size = font.GetSizeOf(text);

		if (size.width > maxWidth)
		{
			maxWidth = size.width;
			maxHeight = size.height;
		}
	}

	uint32_t yOffset = height / 2 - ((maxHeight + 15) * static_cast<uint32_t>(m_HighScoreTable.Size())) / 2;

	for (auto& score : m_HighScoreTable.GetTable())
	{	
		std::string text = score.PlayerName + "   " + std::to_string(score.score);

		theScreen.Draw(font, text, Vec2D(width / 2 - maxWidth / 2, yOffset), Colour::White());

		yOffset += fontSize.height + 20;
	}
}

const std::string& HighScoreScene::GetSceneName() const
{
	static std::string name = "High-Score scene";
	return name;
}

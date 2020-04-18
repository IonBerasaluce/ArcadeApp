#include "GameScene.h"

GameScene::GameScene(std::unique_ptr<Game> optrGame):m_Game(std::move(optrGame))
{
}

void GameScene::Init()
{
	m_Game->Init(m_GameController);
}

void GameScene::Update(uint32_t dt)
{
	m_Game->Update(dt);
}

void GameScene::Draw(Screen& screen)
{
	m_Game->Draw(screen);
}

const std::string& GameScene::GetSceneName() const
{
	return m_Game->GetName();
}

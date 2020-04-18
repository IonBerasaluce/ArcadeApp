#pragma once

#include "ButtonOptionsScene.h"
#include "App/HighScoreTable.h"
#include "Inputs/InputAction.h"

class HighScoreScene : public Scene
{
public:

	HighScoreScene(HighScoreTable table);

	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& theScreen) override;
	virtual const std::string& GetSceneName() const override;

private:
	HighScoreTable m_HighScoreTable;
};
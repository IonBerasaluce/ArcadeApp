#pragma once
#include "Scenes/ButtonOptionsScene.h"


class AsteroidsStartScene :public ButtonOptionsScene
{
public:
	AsteroidsStartScene();
	void Init() override;
	void Update(uint32_t dt) override;
	void Draw(Screen& theScreen) override;
	const std::string& GetSceneName() const override;

private:

};
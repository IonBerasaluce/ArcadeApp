#include "AsteroidsGame.h"
#include "Graphics/Screen.h"
#include "Shapes/Triangle.h"

void AsteroidsGame::Init(GameController& controller)
{
}

void AsteroidsGame::Update(uint32_t dt)
{
}

void AsteroidsGame::Draw(Screen& screen)
{
	Triangle triangle = Triangle(Vec2D(40, 20), Vec2D(20, 60), Vec2D(60, 60));
	screen.Draw(triangle, Colour::White(), false);
}

const std::string& AsteroidsGame::GetName() const
{
	static const std::string name = "Asteroids!";
	return name;
}

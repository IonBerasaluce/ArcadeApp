#pragma once

#include <string>
#include <vector>

#include "Math/Vec2D.h"
#include "Games/Excluder.h"

class Screen;

struct Tile
{
	Vec2D position = Vec2D::Zero;
	int width = 0;
	int collidable = 0;
	char symbol = '-';
};

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath);
	void Update(uint32_t dt);
	void Draw(Screen& screen);

private:

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);

	std::vector<Excluder> m_Walls;
	std::vector<Tile> m_Tiles;

	Vec2D m_LayoutOffset;
	size_t m_TileHeight;
};
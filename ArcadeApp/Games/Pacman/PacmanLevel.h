#pragma once

#include <string>
#include <vector>

#include "Math/Vec2D.h"
#include "Games/Excluder.h"
#include "PacmanGameUtils.h"

class Screen;
class PacmanPlayer;

struct Tile
{
	Vec2D position = Vec2D::Zero;
	Vec2D offset = Vec2D::Zero;
	int width = 0;
	int collidable = 0;
	char symbol = '-';
	char isTeleportTile = 0;
	char teleportToSymbol = 0;
};

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath, PacmanPlayer* ptrPacmanPlayer);
	void Update(uint32_t dt);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement direction) const;

private:

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	PacmanPlayer* m_ptrPacmanPlayer;

	std::vector<Excluder> m_Walls;
	std::vector<Tile> m_Tiles;

	Vec2D m_LayoutOffset;
	size_t m_TileHeight;
};
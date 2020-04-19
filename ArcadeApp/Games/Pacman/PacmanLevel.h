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
	int excludePelletTile = 0;
};

struct Pellet
{
	uint32_t score = 0;
	AARectangle m_BBox;
	int powerPellet = 0;
	int eaten = 0;
};

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath, PacmanPlayer* ptrPacmanPlayer);
	void Update(uint32_t dt);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement direction) const;

	inline Vec2D GetLayoutOffset() const { return m_LayoutOffset; }

private:

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	void ResetLevel();

	PacmanPlayer* m_ptrPacmanPlayer;

	std::vector<Excluder> m_Walls;
	std::vector<Tile> m_Tiles;
	std::vector<Tile> m_ExclusionTiles;
	std::vector<Pellet> m_Pellets;

	Vec2D m_LayoutOffset;
	size_t m_TileHeight;
};
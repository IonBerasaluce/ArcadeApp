#pragma once

#include <string>
#include <vector>
#include <random>

#include "Math/Vec2D.h"
#include "Games/Excluder.h"
#include "PacmanGameUtils.h"
#include "Graphics/SpriteSheet.h"
#include "GhostAI.h"

class Screen;
class PacmanPlayer;
class Ghost;

struct Tile
{
	Vec2D position = Vec2D::Zero;
	Vec2D offset = Vec2D::Zero;
	int width = 0;
	int collidable = 0;
	char symbol = '-';
	char isTeleportTile = 0;
	int pacmanSpawnPoint = 0;
	char teleportToSymbol = 0;
	int excludePelletTile = 0;
	int itemSpawnPoint = 0;

	int blinkySpawnPoint = 0;
	int inkySpawnPoint = 0;
	int pinkySpawnPoint = 0;
	int clydeSpawnPoint = 0;
	int isGate = 0;
};

struct Pellet
{
	uint32_t score = 0;
	AARectangle m_BBox;
	int powerPellet = 0;
	int eaten = 0;
};

struct BonusItem
{
	uint32_t score = 0;
	AARectangle bbox;
	int eaten = 0;
	int spawned = 0;
	int spawnedTime = 1;
};

struct BonusItemLevelProperties
{
	uint32_t score = 0;
	std::string spriteName = "";
	// which level does this item spawn at
	uint32_t begin = 0;
	uint32_t end = 0;
	
};

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath, const SpriteSheet* ptrSpriteSheet);
	void Update(uint32_t dt, PacmanPlayer& pacman, std::vector<Ghost>& ghosts, std::vector<GhostAI>& ghostAIs);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement direction) const;
	bool WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement direction) const;
	inline Vec2D GetLayoutOffset() const { return m_LayoutOffset; }
	inline Vec2D GetPacmanSpawnLocation() const { return m_PacmanSpawnLocation; }
	void ResetLevel();

	bool IsLevelOver() const;
	void IncreaseLevel();
	void ResetToFirstLevel();

	inline const std::vector<Vec2D>& GhostSpawnPoints() { return m_GhostSpawnPoints; }

private:

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	bool HasEatenAllPellets() const;

	void GetBonusItemsSpriteName(std::string& spriteName, uint32_t& score) const;
	void SpawnBonusItem();
	bool ShouldSpawnBonusItem() const;

	size_t NumPelletsEaten() const;

	int m_CurrentLevel;

	std::default_random_engine m_Generator;
	BonusItem m_BonusItem;
	std::string m_BonusItemSpriteName;
	const SpriteSheet* m_ptrSpriteSheet;
	std::vector<BonusItemLevelProperties> m_BonusItemProperties;

	std::vector<Excluder> m_Walls;
	std::vector<Tile> m_Tiles;
	std::vector<Tile> m_ExclusionTiles;
	std::vector<Pellet> m_Pellets;
	std::vector<Vec2D> m_GhostSpawnPoints;
	std::vector<Excluder> m_Gate;

	Vec2D m_PacmanSpawnLocation;
	Vec2D m_LayoutOffset;
	size_t m_TileHeight;
};
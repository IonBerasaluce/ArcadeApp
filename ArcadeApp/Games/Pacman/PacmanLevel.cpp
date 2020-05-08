#include <assert.h>

#include "App/App.h"
#include "Shapes/Circle.h"
#include "PacmanLevel.h"
#include "Utils/FileCommandLoader.h"
#include "Graphics/Screen.h"
#include "PacmanPlayer.h"
#include "Ghost.h"

namespace {
	const uint32_t NUM_LEVELS = 256;
	const uint32_t SPRITE_HEIGHT = 16;
	const uint32_t SPRITE_WIDTH = 16;
}

bool PacmanLevel::Init(const std::string& levelPath, const SpriteSheet* ptrSpriteSheet)
{
	m_CurrentLevel = 0;
	m_ptrSpriteSheet = ptrSpriteSheet;
	m_BonusItemSpriteName = "";
	std::random_device r;
	m_Generator.seed(r());

	m_GhostSpawnPoints.resize(NUM_GHOSTS);

	bool levelLoaded = LoadLevel(levelPath);

	if (levelLoaded)
	{
		ResetLevel();
	}

	return levelLoaded;
}

void PacmanLevel::Update(uint32_t dt, PacmanPlayer& pacman, std::vector<Ghost>& ghosts, std::vector<GhostAI>& ghostAIs)
{
	for (const auto& wall : m_Walls)
	{
		// handling of player collisions
		BoundaryEdge edge;
		if (wall.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Vec2D offset = wall.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			// If pacman collides with a wall we stop him
			pacman.Stop();
		}

		// Handle ghosts collision
		for (auto& ghost : ghosts)
		{
			if (wall.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Vec2D offset = wall.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				// If pacman collides with a wall we stop him
				ghost.Stop();
			}
		}
	}

	for (const auto& gate : m_Gate)
	{
		BoundaryEdge edge;

		// Make sure the gate as a wall for pacman
		if (gate.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Vec2D offset = gate.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (size_t i = 0; i < GhostName::NUM_GHOSTS; i++)
		{
			GhostAI& ghostAI = ghostAIs[i];
			Ghost& ghost = ghosts[i];
			
			// Same as we do for pacman, if the ghosts are not meant to interact with the gate, the gate acts as a wall.
			if (!(ghostAI.WantsToLeavePen() || ghostAI.IsEnteringPen()) && gate.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Vec2D offset = gate.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				ghost.Stop();
			}
		}
	}

	// Handle teleporting of pacman between the levels
	for (Tile t : m_Tiles)
	{
		if (t.isTeleportTile)
		{
			AARectangle teleportTileAABB(t.position, t.width, static_cast<unsigned int>(m_TileHeight));

			Tile* teleportToTile = GetTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);

			if (teleportToTile->isTeleportTile)
			{
				if (teleportTileAABB.Intersects(pacman.GetBoundingBox()))
				{
					pacman.MoveTo(teleportToTile->position + teleportToTile->offset);
				}

				for (auto& ghost : ghosts)
				{
					if (teleportTileAABB.Intersects(ghost.GetBoundingBox()))
					{
						ghost.MoveTo(teleportToTile->position + teleportToTile->offset);
					}
				}
			}
		}
	}

for (auto& pellet : m_Pellets)
{
	if (!pellet.eaten)
	{
		if (pacman.GetEatingBoundingBox().Intersects(pellet.m_BBox))
		{
			pellet.eaten = true;
			pacman.AteItem(pellet.score);

			if (pellet.powerPellet)
			{
				pacman.ResetGhostEatenMultiplier();
				for (auto& ghost : ghosts)
				{
					ghost.SetStateToVulnerable();
				}
			}
		}
	}
}

if (ShouldSpawnBonusItem())
{
	SpawnBonusItem();
}

if (m_BonusItem.spawned && !m_BonusItem.eaten)
{
	if (pacman.GetEatingBoundingBox().Intersects(m_BonusItem.bbox))
	{
		m_BonusItem.eaten = true;
		pacman.AteItem(m_BonusItem.score);
	}
}
}

void PacmanLevel::Draw(Screen& screen)
{
	Sprite bgSprite;
	bgSprite.width = m_BGImage.GetWidth();
	bgSprite.height = m_BGImage.GetHeight();

	screen.Draw(m_BGImage, bgSprite, Vec2D::Zero);

	// Drawing the pellets
	for (const auto& pellet : m_Pellets)
	{
		if (!pellet.eaten)
		{
			if (!pellet.powerPellet)
			{
				screen.Draw(pellet.m_BBox, Colour::White());
			}
			else
			{
				Circle c(pellet.m_BBox.GetCenterPoint(), pellet.m_BBox.GetWidth() / 2.0f);
				screen.Draw(c, Colour::White(), true, Colour::White());
			}
		}
	}

	// Draw the bonus items if they are not eaten
	if (m_BonusItem.spawned && !m_BonusItem.eaten)
	{
		screen.Draw(*m_ptrSpriteSheet, m_BonusItemSpriteName, m_BonusItem.bbox.GetTopLeft());
	}
}

bool PacmanLevel::WillCollide(const AARectangle& aBBox, PacmanMovement direction) const
{
	AARectangle bbox = aBBox;

	bbox.MoveBy(GetMovementVector(direction));

	BoundaryEdge edge;

	for (const auto& wall : m_Walls)
	{
		if (wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	for (const auto& gate : m_Gate)
	{
		if (gate.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;
}

bool PacmanLevel::WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement direction) const
{
	AARectangle bbox = ghost.GetBoundingBox();

	bbox.MoveBy(GetMovementVector(direction));

	BoundaryEdge edge;
	for (const auto& wall : m_Walls)
	{
		if (wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	// Handle collisions with the gate - if the ghost is hitting the gate and is not in entering or leving the pen states then a collision happens
	for (const auto& gate : m_Gate)
	{
		if (!(ghostAI.IsEnteringPen() || ghostAI.WantsToLeavePen()) && gate.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;
}

void PacmanLevel::ResetLevel()
{
	ResetPellets();

	std::uniform_int_distribution<size_t> distribution(10);
	m_BonusItem.spawnedTime = static_cast<int>(distribution(m_Generator));

	GetBonusItemsSpriteName(m_BonusItemSpriteName, m_BonusItem.score);
}

bool PacmanLevel::IsLevelOver() const
{
	return HasEatenAllPellets();
}

void PacmanLevel::IncreaseLevel()
{
	m_CurrentLevel++;

	if (m_CurrentLevel > NUM_LEVELS)
	{
		m_CurrentLevel = 1;
	}

	ResetLevel();
}

void PacmanLevel::ResetToFirstLevel()
{
	m_CurrentLevel = 1;
	ResetLevel();
}

void PacmanLevel::ResetPellets()
{
	m_Pellets.clear();

	// TODO: clean up this mess of constants
	const uint32_t PELLET_SIZE = 2; 
	const uint32_t PADDING = static_cast<uint32_t>(m_TileHeight);

	uint32_t startingY = (uint32_t)m_LayoutOffset.GetY() + PADDING + m_TileHeight - 1;
	uint32_t startingX = PADDING + 3;

	const uint32_t LEVEL_HEIGHT = (uint32_t)m_LayoutOffset.GetY() + 32 * m_TileHeight;
	
	Pellet p; 
	p.score = 10;
	uint32_t row = 0;
	
	//// Loop through the entire map
	for (uint32_t y = startingY; y < LEVEL_HEIGHT; y += PADDING, ++row)
	{
		for (uint32_t x = startingX, col = 0; x < App::Singleton().Width(); x += PADDING, ++col)
		{
			// Check for power pellet positions 
			if (row == 2 || row == 22)
			{
				if (col == 0 || col == 25)
				{
					p.powerPellet = 1;
					p.score = 0;
					p.m_BBox = AARectangle(Vec2D((float)x - 3.0f, (float)y - 3.0f), m_TileHeight, m_TileHeight);
					m_Pellets.push_back(p);

					p.powerPellet = 0;
					p.score = 10;

					continue;
				}
			}

			// Make a pellet
			AARectangle rect = AARectangle(Vec2D((float)x, (float)y), PELLET_SIZE, PELLET_SIZE);

			bool found = false;

			// Check that we are not hitting any walls
			for (const Excluder& wall : m_Walls)
			{
				if (wall.GetAARectangle().Intersects(rect))
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				// Check that we are not in the excluded tiles
				for (const Tile& excludePelletTile : m_ExclusionTiles)
				{
					if (excludePelletTile.excludePelletTile)
					{
						AARectangle tileAABB(excludePelletTile.position, excludePelletTile.width, excludePelletTile.width);

						if (tileAABB.Intersects(rect))
						{
							found = true;
							break;
						}
					}
				}
			}
			// If the position is not a wall nor a teleport tile we can place pellet 
			if (!found)
			{
				p.m_BBox = rect;
				m_Pellets.push_back(p);
			}
		}
	}
}

bool PacmanLevel::HasEatenAllPellets() const
{
	// 4 superpellets donn't have to be eaten to complete the level.
	return NumPelletsEaten() >= m_Pellets.size() - 4;
}

void PacmanLevel::GetBonusItemsSpriteName(std::string& spriteName, uint32_t& score) const
{
	for (const auto& properties : m_BonusItemProperties)
	{
		if (m_CurrentLevel >= properties.begin && m_CurrentLevel <= properties.end)
		{
			spriteName = properties.spriteName;
			score = properties.score;
			return;
		}
	}
}

void PacmanLevel::SpawnBonusItem()
{
	m_BonusItem.spawned = 1;
	m_BonusItem.eaten = 0;
}

bool PacmanLevel::ShouldSpawnBonusItem() const
{
	auto numEaten = NumPelletsEaten();
	return !m_BonusItem.spawned && numEaten >= m_BonusItem.spawnedTime;
}

size_t PacmanLevel::NumPelletsEaten() const
{
	size_t numEaten = 0;

	for (const auto& pellet : m_Pellets)
	{
		if (!pellet.powerPellet && pellet.eaten)
		{
			++numEaten;
		}
	}
	
	return numEaten;
}

bool PacmanLevel::LoadLevel(const std::string& levelPath)
{
	FileCommandLoader fileLoader;

	std::string bgImageName;

	Command bgImageCommand;
	bgImageCommand.command = "bg_image";
	bgImageCommand.parseFunc = [this, &bgImageName](ParseFunctionParams params)
	{
		bgImageName = FileCommandLoader::ReadString(params);
		bool loaded = m_BGImage.Load(App::Singleton().GetBasePath() + std::string("Assets\\") + bgImageName);

		assert(loaded && "Didn't load the BG - Image");
	};

	fileLoader.AddCommand(bgImageCommand);

	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().width = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileWidthCommand);

	Command tileHeightCommand;
	tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_TileHeight = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileHeightCommand);

	Command tileCommand;
	tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.push_back(Tile());
	};

	fileLoader.AddCommand(tileCommand);

	Command tileSymbolCommand;
	tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().symbol = FileCommandLoader::ReadChar(params);
	};

	fileLoader.AddCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_collision";
	tileCollisionCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().collidable = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileCollisionCommand);

	Vec2D layoutOffset;

	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFunctionParams params)
	{
		m_LayoutOffset = FileCommandLoader::ReadSize(params);
		layoutOffset = m_LayoutOffset;
	};

	fileLoader.AddCommand(layoutOffsetCommand);

	Command tileExcludePelletCommand;
	tileExcludePelletCommand.command = "tile_exclude_pellet";
	tileExcludePelletCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().excludePelletTile = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileExcludePelletCommand);

	// Teleporting tiles
	Command tileToTeleportToCommand;
	tileToTeleportToCommand.command = "tile_teleport_to_symbol";
	tileToTeleportToCommand.parseFunc = [&layoutOffset, this](ParseFunctionParams params)
	{
		m_Tiles.back().teleportToSymbol = FileCommandLoader::ReadChar(params);
	};

	fileLoader.AddCommand(tileToTeleportToCommand);

	Command tileIsTeleportTileCommand;
	tileIsTeleportTileCommand.command = "tile_is_teleport_tile";
	tileIsTeleportTileCommand.parseFunc = [&layoutOffset, this](ParseFunctionParams params)
	{
		m_Tiles.back().isTeleportTile = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileIsTeleportTileCommand);

	Command tileOffsetCommand;
	tileOffsetCommand.command = "tile_offset";
	tileOffsetCommand.parseFunc = [&layoutOffset, this](ParseFunctionParams params)
	{
		m_Tiles.back().offset = FileCommandLoader::ReadSize(params);
	};

	fileLoader.AddCommand(tileOffsetCommand);

	Command tilePacmanSpawnPointCommand;
	tilePacmanSpawnPointCommand.command = "tile_pacman_spawn_point";
	tilePacmanSpawnPointCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().pacmanSpawnPoint = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tilePacmanSpawnPointCommand);

	// Bonus items 
	Command tileItemSpawnPointCommand;
	tileItemSpawnPointCommand.command = "tile_item_spawn_point";
	tileItemSpawnPointCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().itemSpawnPoint = FileCommandLoader::ReadInt(params);
	};
	
	fileLoader.AddCommand(tileItemSpawnPointCommand);

	// Ghosts Spawn tiles
	Command tileBlinkySpawnPointCommand;
	tileBlinkySpawnPointCommand.command = "tile_blinky_spawn_point";
	tileBlinkySpawnPointCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().blinkySpawnPoint = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileBlinkySpawnPointCommand);

	Command tileInkySpawnPointCommand;
	tileInkySpawnPointCommand.command = "tile_inky_spawn_point";
	tileInkySpawnPointCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().inkySpawnPoint = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileInkySpawnPointCommand);

	Command tilePinkySpawnPointCommand;
	tilePinkySpawnPointCommand.command = "tile_pinky_spawn_point";
	tilePinkySpawnPointCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().pinkySpawnPoint = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tilePinkySpawnPointCommand);

	Command tileClydeSpawnPointCommand;
	tileClydeSpawnPointCommand.command = "tile_clyde_spawn_point";
	tileClydeSpawnPointCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().clydeSpawnPoint = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileClydeSpawnPointCommand);

	Command tileGateCommand;
	tileGateCommand.command = "tile_is_gate";
	tileGateCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_Tiles.back().isGate = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileGateCommand);

	// entire layout
	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFunctionParams params)
	{
		int startingX = (int)layoutOffset.GetX();
		
		// this does the lines
		for (int c = 0; c < params.line.length(); c++)
		{
			Tile* tile = GetTileForSymbol(params.line[c]);

			if (tile)
			{
				tile->position = Vec2D((float)startingX, layoutOffset.GetY());

				if (tile->isGate > 0)
				{
					Excluder gate;
					gate.Init(AARectangle(Vec2D((float)startingX, layoutOffset.GetY()), tile->width, static_cast<unsigned int>(m_TileHeight)));
					m_Gate.push_back(gate);
				}
				else if (tile->collidable > 0)
				{
					Excluder wall;
					wall.Init(AARectangle(Vec2D((float)startingX, layoutOffset.GetY()), tile->width, static_cast<unsigned int>(m_TileHeight)));

					m_Walls.push_back(wall);
				}

				if (tile->pacmanSpawnPoint > 0)
				{
					m_PacmanSpawnLocation = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->itemSpawnPoint > 0)
				{
					m_BonusItem.bbox = AARectangle(Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY()), SPRITE_WIDTH, SPRITE_HEIGHT);
				}
				else if (tile->blinkySpawnPoint > 0)
				{
					m_GhostSpawnPoints[BLINKY] = Vec2D(startingX + tile->offset.GetX() + 1, layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->inkySpawnPoint > 0)
				{
					m_GhostSpawnPoints[INKY] = Vec2D(startingX + tile->offset.GetX() + 1, layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->pinkySpawnPoint > 0)
				{
					m_GhostSpawnPoints[PINKY] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->clydeSpawnPoint > 0)
				{
					m_GhostSpawnPoints[CLYDE] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				
				if (tile->excludePelletTile > 0)
				{
					m_ExclusionTiles.push_back(*tile);
				}

				startingX += tile->width;
			}
		}

		// Once we are done with the line we offset the column
		layoutOffset += Vec2D(0, (float)m_TileHeight);
	};

	fileLoader.AddCommand(layoutCommand);

	Command bonusItemCommand;
	bonusItemCommand.command = "bonus_item";
	bonusItemCommand.parseFunc = [this](ParseFunctionParams params)
	{
		BonusItemLevelProperties newProperty;
		m_BonusItemProperties.push_back(newProperty);
	};

	fileLoader.AddCommand(bonusItemCommand);

	Command bonusItemSpriteNameCommand;
	bonusItemSpriteNameCommand.command = "bonus_item_sprite_name";
	bonusItemSpriteNameCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_BonusItemProperties.back().spriteName = FileCommandLoader::ReadString(params);
	};

	fileLoader.AddCommand(bonusItemSpriteNameCommand);


	Command bonusItemScoreCommand;
	bonusItemScoreCommand.command = "bonus_item_score";
	bonusItemScoreCommand.parseFunc = [this](ParseFunctionParams params)
	{
		m_BonusItemProperties.back().score = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(bonusItemScoreCommand);

	Command bonusItemBeginLevel;
	bonusItemBeginLevel.command = "bonus_item_begin_level";
	bonusItemBeginLevel.parseFunc = [this](ParseFunctionParams params)
	{
		m_BonusItemProperties.back().begin = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(bonusItemBeginLevel);

	Command bonusItemEndLevel;
	bonusItemEndLevel.command = "bonus_item_end_level";
	bonusItemEndLevel.parseFunc = [this](ParseFunctionParams params)
	{
		m_BonusItemProperties.back().end = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(bonusItemEndLevel);

	return fileLoader.LoadFile(levelPath);
}

Tile* PacmanLevel::GetTileForSymbol(char symbol)
{
	for (size_t i = 0; i < m_Tiles.size(); ++i)
	{
		if (m_Tiles[i].symbol == symbol)
		{
			return &m_Tiles[i];
		}
	}

	return nullptr;
}

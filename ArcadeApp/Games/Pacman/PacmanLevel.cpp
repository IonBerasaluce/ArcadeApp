#include <assert.h>

#include "App/App.h"
#include "Shapes/Circle.h"
#include "PacmanLevel.h"
#include "Utils/FileCommandLoader.h"
#include "Graphics/Screen.h"
#include "PacmanPlayer.h"

bool PacmanLevel::Init(const std::string& levelPath, PacmanPlayer* ptrPacmanPlayer)
{
	m_ptrPacmanPlayer = ptrPacmanPlayer;

	bool levelLoaded = LoadLevel(levelPath);
	if (levelLoaded)
	{
		ResetLevel();
	}

	return levelLoaded;
}

void PacmanLevel::Update(uint32_t dt)
{
	for (const auto& wall : m_Walls)
	{
		BoundaryEdge edge;
		if (wall.HasCollided(m_ptrPacmanPlayer->GetBoundingBox(), edge))
		{
			Vec2D offset = wall.GetCollisionOffset(m_ptrPacmanPlayer->GetBoundingBox());
			m_ptrPacmanPlayer->MoveBy(offset);
			// If pacman collides with a wall we stop him
			m_ptrPacmanPlayer->Stop();
		}
	}

	// Handle teleporting of pacman between the levels
	for (Tile t : m_Tiles)
	{
		if (t.isTeleportTile)
		{
			AARectangle teleportTileAABB(t.position, t.width, static_cast<float>(m_TileHeight));

			Tile* teleportToTile = GetTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);

			if (teleportToTile->isTeleportTile && teleportTileAABB.Intersects(m_ptrPacmanPlayer->GetBoundingBox()))
			{
				m_ptrPacmanPlayer->MoveTo(teleportToTile->position + teleportToTile->offset);
			}
		}
	}

	for (auto& pellet : m_Pellets)
	{
		if (!pellet.eaten)
		{
			if (m_ptrPacmanPlayer->GetEatingBoundingBox().Intersects(pellet.m_BBox))
			{
				pellet.eaten = true;
				m_ptrPacmanPlayer->AteItem(pellet.score);

				if (pellet.powerPellet)
				{
					m_ptrPacmanPlayer->ResetGhostEatenMultiplier();
					// TODO: Change the ghosts state to vulnerable
				}
			}
		}
	}
}

void PacmanLevel::Draw(Screen& screen)
{
	 //Debug code for wall check
	for (const auto& wall : m_Walls)
	{
		screen.Draw(wall.GetAARectangle(), Colour::Blue());
	}

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
}

bool PacmanLevel::WillCollide(const AARectangle& aBBox, PacmanMovement direction) const
{
	AARectangle bbox = aBBox;

	bbox.MoveBy(GetMovementVector(direction));

	for (const auto& wall : m_Walls)
	{
		BoundaryEdge edge;
		if (wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}
	return false;
}

void PacmanLevel::ResetLevel()
{
	ResetPellets();
}

void PacmanLevel::ResetPellets()
{
	m_Pellets.clear();

	// TODO: clean up this mess of constants
	const uint32_t PELLET_SIZE = 2; 
	const uint32_t PADDING = static_cast<uint32_t>(m_TileHeight);

	uint32_t startingY = m_LayoutOffset.GetY() + PADDING + m_TileHeight - 1;
	uint32_t startingX = PADDING + 3;

	const uint32_t LEVEL_HEIGHT = m_LayoutOffset.GetY() + 32 * m_TileHeight;
	
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
					p.m_BBox = AARectangle(Vec2D(x - 3, y - 3), m_TileHeight, m_TileHeight);
					m_Pellets.push_back(p);

					p.powerPellet = 0;
					p.score = 10;

					continue;
				}
			}

			// Make a pellet
			AARectangle rect = AARectangle(Vec2D(x, y), PELLET_SIZE, PELLET_SIZE);

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

bool PacmanLevel::LoadLevel(const std::string& levelPath)
{
	FileCommandLoader fileLoader;

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

	// entire layout
	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFunctionParams params)
	{
		int startingX = layoutOffset.GetX();
		
		// this does the lines
		for (int c = 0; c < params.line.length(); c++)
		{
			Tile* tile = GetTileForSymbol(params.line[c]);

			if (tile)
			{
				tile->position = Vec2D(startingX, layoutOffset.GetY());

				if (tile->collidable > 0)
				{
					Excluder wall;
					wall.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<unsigned int>(m_TileHeight)));

					m_Walls.push_back(wall);
				}

				if (tile->excludePelletTile > 0)
				{
					m_ExclusionTiles.push_back(*tile);
				}

				startingX += tile->width;
			}
		}

		// Once we are done with the line we offset the column
		layoutOffset += Vec2D(0, m_TileHeight);
	};

	fileLoader.AddCommand(layoutCommand);

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

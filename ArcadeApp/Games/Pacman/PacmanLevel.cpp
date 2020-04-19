#include <assert.h>
#include "PacmanLevel.h"
#include "Utils/FileCommandLoader.h"
#include "Graphics/Screen.h"
#include "PacmanPlayer.h"

bool PacmanLevel::Init(const std::string& levelPath, PacmanPlayer* ptrPacmanPlayer)
{
	m_ptrPacmanPlayer = ptrPacmanPlayer;
	return LoadLevel(levelPath);
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
}

void PacmanLevel::Draw(Screen& screen)
{
	// Debug code for wall check
	for (const auto& wall : m_Walls)
	{
		screen.Draw(wall.GetAARectangle(), Colour::Blue());
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

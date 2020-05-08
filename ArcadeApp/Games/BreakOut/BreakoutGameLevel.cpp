#include "BreakoutGameLevel.h"
#include "Ball.h"
#include "App/App.h"
#include "Utils/FileCommandLoader.h"
#include "App/HighScoreTable.h"

struct LayoutBlock
{
	char symbol = '-';
	int hp = 0;
	Colour colour = Colour::Black();
};

BreakoutGameLevel::BreakoutGameLevel()
{
}

void BreakoutGameLevel::Init(const AARectangle& boundary)
{
	CreateDefaultLevel(boundary);
}

void BreakoutGameLevel::Load(const std::vector<Block>& blocks)
{
	m_Blocks.clear();
	m_Blocks = blocks;
}

void BreakoutGameLevel::Update(uint32_t dt, Ball& ball, ScoreInformation& score)
{
	std::vector<Block> collidedBlocks;
	BoundaryEdge edgeToBounceOffOf;
	Block* noptrBlocktoBounceOffOf = nullptr;

	float largestMag = -1.0f;

	for (auto& block : m_Blocks)
	{
		BoundaryEdge edge;

		if (!block.IsDestroyed() && block.HasCollided(ball.GetBoundingRect(), edge))
		{
			collidedBlocks.push_back(block);
			float mag = block.GetCollisionOffset(ball.GetBoundingRect()).Mag();

			if (mag > largestMag)
			{
				edgeToBounceOffOf = edge;
				noptrBlocktoBounceOffOf = &block;
			}
		}
	}

	if (noptrBlocktoBounceOffOf != nullptr)
	{
		noptrBlocktoBounceOffOf->Bounce(ball, edgeToBounceOffOf);
		noptrBlocktoBounceOffOf->ReduceHP();
		score.score += 10;

	}

	for (const auto& block : collidedBlocks)
	{
		BoundaryEdge edge;
		if (block.HasCollided(ball.GetBoundingRect(), edge))
		{
			Vec2D p;
			ball.MakeFlushWithEdge(edge, p, true);
		}
	}
}

void BreakoutGameLevel::Draw(Screen& screen)
{
	for (auto& block : m_Blocks)
	{
		if (!block.IsDestroyed())
		{
			block.Draw(screen);
		}
	}
}

bool BreakoutGameLevel::IsLevelComplete() const
{
	for (size_t i = 0; i < m_Blocks.size(); i++)
	{
		if (!m_Blocks[i].IsDestroyed() && m_Blocks[i].GetHP() != Block::UNBREAKABLE)
		{
			return false;
		}
	}
	return true;
}


void BreakoutGameLevel::CreateDefaultLevel(const AARectangle& boundary)
{
	m_Blocks.clear();

	//const int NUM_BLOCKS_ACROSS = ((int)boundary.GetWidth() - (2 * (BLOCK_WIDTH)) / BLOCK_WIDTH);
	const int NUM_BLOCKS_ACROSS = (int)boundary.GetWidth() / BLOCK_WIDTH;
	const int NUM_BLOCK_ROWS = 5;
	float startX = boundary.GetTopLeft().GetX();

	Colour colours[NUM_BLOCK_ROWS];
	colours[0] = Colour::Red();
	colours[1] = Colour::Magenta();
	colours[2] = Colour::Yellow();
	colours[3] = Colour::Green();
	colours[4] = Colour::Cyan();


	for (int r = 0; r < NUM_BLOCK_ROWS; ++r)
	{
		AARectangle blockRect = { Vec2D(startX, (float)(BLOCK_HEIGHT * (r + 1))), BLOCK_WIDTH, BLOCK_HEIGHT };

		for (int c = 0; c  < NUM_BLOCKS_ACROSS; c ++)
		{
			Block b;
			b.Init(blockRect, 1, Colour::Black(), colours[r]);
			m_Blocks.push_back(b);
			blockRect.MoveBy(Vec2D(BLOCK_WIDTH, 0));
		}
	}
}

LayoutBlock FindLayoutBlockForSymbol(const std::vector<LayoutBlock>& blocks, char symbol)
{
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].symbol == symbol)
		{
			return blocks[i];
		}
	}

	return LayoutBlock();
}


std::vector<BreakoutGameLevel> BreakoutGameLevel::LoadLevelsFromFile(const std::string& filePath)
{
	std::vector<BreakoutGameLevel> levels;
	std::vector<LayoutBlock> layoutBlocks;
	std::vector<Block> levelBlocks;

	int width = 0;
	int height = 0;

	FileCommandLoader fileLoader;

	Command levelCommand;
	levelCommand.command = "level";
	levelCommand.parseFunc = [&](ParseFunctionParams params)
	{
		if (levels.size() > 0)
		{
			levels.back().Load(levelBlocks);
		}
		layoutBlocks.clear();
		levelBlocks.clear();

		width = 0;
		height = 0;

		BreakoutGameLevel level;
		level.Init(AARectangle(Vec2D::Zero, App::Singleton().Width(), App::Singleton().Height()));

		levels.push_back(level);
	};

	fileLoader.AddCommand(levelCommand);

	Command blockCommand;
	blockCommand.command = "block";
	blockCommand.parseFunc = [&](ParseFunctionParams params)
	{
		LayoutBlock lb;
		layoutBlocks.push_back(lb);
	};

	fileLoader.AddCommand(blockCommand);

	Command symbolCommand;
	symbolCommand.command = "symbol";
	symbolCommand.parseFunc = [&](ParseFunctionParams params)
	{
		layoutBlocks.back().symbol = FileCommandLoader::ReadChar(params);
	};

	fileLoader.AddCommand(symbolCommand);

	Command fillColourCommand;
	fillColourCommand.command = "fillcolor";
	fillColourCommand.parseFunc = [&](ParseFunctionParams params)
	{
		layoutBlocks.back().colour = FileCommandLoader::ReadColour(params);
	};

	fileLoader.AddCommand(fillColourCommand);

	Command hpCommand;
	hpCommand.command = "hp";
	hpCommand.parseFunc = [&](ParseFunctionParams params)
	{
		layoutBlocks.back().hp = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(hpCommand);

	Command widthCommand;
	widthCommand.command = "width";
	widthCommand.parseFunc = [&](ParseFunctionParams params)
	{
		width = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(widthCommand);

	Command heightCommand;
	heightCommand.command = "height";
	heightCommand.parseFunc = [&](ParseFunctionParams params)
	{
		height = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(heightCommand);

	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&](ParseFunctionParams params)
	{
		int blockWidth = BLOCK_WIDTH;
		int screenWidth = App::Singleton().Width();

		float startingX = 0;
		AARectangle blockRect(Vec2D(startingX, (float)((params.lineNum + 1) * BLOCK_HEIGHT)), BLOCK_WIDTH, BLOCK_HEIGHT);

		for (int c = 0; c < params.line.length(); c++)
		{
			if (params.line[c] != '-')
			{
				LayoutBlock layoutBlock = FindLayoutBlockForSymbol(layoutBlocks, params.line[c]);
				Block b;

				b.Init(blockRect, layoutBlock.hp, Colour::Black(), layoutBlock.colour);
				levelBlocks.push_back(b);
			}
			blockRect.MoveBy(Vec2D(BLOCK_WIDTH, 0));
		}
	};

	fileLoader.AddCommand(layoutCommand);

	fileLoader.LoadFile(filePath);

	if (levels.size() > 0)
	{
		levels.back().Load(levelBlocks);
	}

	return levels;
}
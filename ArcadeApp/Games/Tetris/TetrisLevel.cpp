#include "TetrisLevel.h"
#include "Graphics/Colour.h"
#include "Graphics/Screen.h"
#include "Math/Vec2D.h"
#include "App/App.h"


TetrisLevel::TetrisLevel(): m_PlayingField{ 0 }
{
}

// Here wee add an integer parameter to pass by referemce the score which we can then save to m_Score
void TetrisLevel::Init(const Vec2D& startPosition)
{
	m_LevelBoundary = { startPosition, LEVEL_WIDTH * Tetromino::BLOCK_WIDTH, LEVEL_HEIGHT * Tetromino::BLOCK_HEIGHT};
	for (size_t i = 0; i < LEVEL_HEIGHT * LEVEL_WIDTH; i++)
	{
		// Clear the boundary
		m_PlayingField[i] = 0;
	}
	m_Lines = 0;
	m_Font = App::Singleton().GetFont();
}

void TetrisLevel::Update(uint32_t dt)
{
	/*
	1. Loop through the array, if there is a given row where all values are not 0, set them all to 0
	2. Move the aray down by changing the index
	*/
	std::vector<int> lines;
	bool bLine;
	for (size_t y = 0; y < LEVEL_HEIGHT; y++)
	{
		bLine = true;
		for (size_t x = 0; x < LEVEL_WIDTH; x++)
		{
			bLine &= (m_PlayingField[y * LEVEL_WIDTH + x]) != 0;
		}

		// If the line is full set the values to zero again
		if (bLine)
		{
			for (size_t i = 0; i < LEVEL_WIDTH; i++)
			{
				m_PlayingField[y * LEVEL_WIDTH + i] = 0;
			}
			lines.push_back(y);
		}
	}

	if (!lines.empty())
	{
		for (const auto& v : lines) 
		{
			for (int x = 0; x < LEVEL_WIDTH; x++)
			{
				// Move the entire grid down 
				for (size_t y = v; y > 0; y--)
				{
					m_PlayingField[y * LEVEL_WIDTH + x] = m_PlayingField[(y - 1) * LEVEL_WIDTH + x];
				}
				// Fill the top row
				m_PlayingField[x] = 0;
			}
		}

		// Add to the score
		m_Lines += lines.size();
	}
}

void TetrisLevel::Draw(Screen& theScreen)
{
	// Draw the playing field 
	Colour colours[7] = { Colour::Cyan(), Colour::Blue(), Colour::Orange(), Colour::Yellow(), Colour::Green(), Colour::Pink(), Colour::Red() };

	for (size_t i = 0; i < 200; i++)
	{
		if (m_PlayingField[i] != 0)
		{
			// transform the index into the x and y coordinates.
			uint32_t y = i / LEVEL_WIDTH;
			uint32_t x = i - y * LEVEL_WIDTH;

			Vec2D position = { static_cast<float>(x * Tetromino::BLOCK_WIDTH + m_LevelBoundary.GetTopLeft().GetX()), static_cast<float>(y * Tetromino::BLOCK_HEIGHT + m_LevelBoundary.GetTopLeft().GetY())  };
			AARectangle rect = {  position, Tetromino::BLOCK_WIDTH, Tetromino::BLOCK_HEIGHT };
			theScreen.Draw(rect, colours[m_PlayingField[i] - 1], true, colours[m_PlayingField[i] - 1]);
		}
	}

	// Draw the boundaries
	AARectangle newPieceEnclosure, scoreEnclosure;
	theScreen.Draw(m_LevelBoundary, Colour::White(), false);

	newPieceEnclosure = { Vec2D(m_LevelBoundary.GetBottomRight().GetX() + 10, m_LevelBoundary.GetTopLeft().GetY()), 6 * Tetromino::BLOCK_WIDTH, 4 * Tetromino::BLOCK_HEIGHT };
	theScreen.Draw(newPieceEnclosure, Colour::White(), false);
	
	float start = newPieceEnclosure.GetBottomRight().GetY() + 5;

	// Write the score and the number of lines completed
	std::string textToPrint[3] = { "Level:  " + std::to_string(1), "Lines:  " + std::to_string(m_Lines), "Score:  " + std::to_string(0) };
	

	// Draw the Rectangle for the next piece, score, level and lines completed
	for (size_t i = 0; i < 3; i++)
	{
		scoreEnclosure = { Vec2D(newPieceEnclosure.GetTopLeft().GetX(), start), 6 * Tetromino::BLOCK_WIDTH, 2 * Tetromino::BLOCK_HEIGHT };
		theScreen.Draw(scoreEnclosure, Colour::White(), false);

		Vec2D textDrawPosition = m_Font.GetDrawPosition(textToPrint[i], scoreEnclosure, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE);

		theScreen.Draw(m_Font, textToPrint[i], textDrawPosition, Colour::White());

		start = scoreEnclosure.GetBottomRight().GetY();
	}
}

void TetrisLevel::AddPiece(const Tetromino& newPiece)
{
	// Get the piece location
	std::vector<AARectangle> signficantBlocks = newPiece.GetSignificantBlocks();
	float xCoord, yCoord;

	for (size_t i = 0; i < 4; i++)
	{
		int index = GetLevelIndex(signficantBlocks[i].GetTopLeft().GetX(), signficantBlocks[i].GetTopLeft().GetY());

		m_PlayingField[index] = static_cast<int>(newPiece.GetPieceType()) + 1;
	}
}

bool TetrisLevel::DoesPieceFit(Tetromino pieceToFit, const Vec2D& vector, int rotation)
{
	// We move the piece the modified location
	pieceToFit.Rotate(rotation);
	pieceToFit.MoveBy(vector);

	std::vector<uint32_t> significantIndices = pieceToFit.GetSignificantIndices();
	std::vector<AARectangle> pieceBlocks = pieceToFit.GetPieceBlocks();

	float xMinLoc, xMaxLoc, yMinLoc, yMaxLoc;

	for (const auto& i : significantIndices)
	{
		// Check if block is within the bounds of the field
		xMinLoc = pieceBlocks[i].GetTopLeft().GetX();
		xMaxLoc = pieceBlocks[i].GetBottomRight().GetX();
		yMinLoc = pieceBlocks[i].GetTopLeft().GetY();
		yMaxLoc = pieceBlocks[i].GetBottomRight().GetY();

		uint32_t levelIndex = GetLevelIndex(xMinLoc, yMinLoc);
		
		if (xMinLoc >= m_LevelBoundary.GetTopLeft().GetX() && xMaxLoc <= m_LevelBoundary.GetBottomRight().GetX())
		{
			if (yMinLoc >= m_LevelBoundary.GetTopLeft().GetY() && yMaxLoc <= m_LevelBoundary.GetBottomRight().GetY())
			{
				if (m_PlayingField[levelIndex] != 0) {
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

uint32_t TetrisLevel::GetLevelIndex(float x, float y)
{
	uint32_t xCoord = static_cast<uint32_t>(x - m_LevelBoundary.GetTopLeft().GetX()) / Tetromino::BLOCK_WIDTH;
	uint32_t yCoord = static_cast<uint32_t>(y - m_LevelBoundary.GetTopLeft().GetY()) / Tetromino::BLOCK_HEIGHT;

	int index = yCoord * LEVEL_WIDTH + xCoord;
	return index;
}
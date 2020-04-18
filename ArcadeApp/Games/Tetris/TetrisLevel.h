#pragma once

#include "Tetromino.h"
#include "Shapes/AARectangle.h"
#include "Graphics/BitmapFont.h"

class Screen;

// Contains the boundary, playing field and all of the pieces that have fallen and stopped

class TetrisLevel
{

public:
	static const uint8_t LEVEL_WIDTH = 10;
	static const uint8_t LEVEL_HEIGHT = 20;
	//static const Colour colours[7];

	TetrisLevel();
	~TetrisLevel() {};

	void Init(const Vec2D& startPosition);
	void Update(uint32_t dt);
	void Draw(Screen& theScreen);

	void AddPiece(const Tetromino& newPiece);
	bool DoesPieceFit(Tetromino pieceToFit, const Vec2D& vector, int rotation);

	inline const AARectangle GetLevelBoundary() const { return m_LevelBoundary; }

private:

	uint32_t GetLevelIndex(float x, float y);
	uint32_t m_PlayingField[LEVEL_WIDTH * LEVEL_HEIGHT];

	AARectangle m_LevelBoundary;

	uint32_t m_Lines;

	BitmapFont m_Font;
};
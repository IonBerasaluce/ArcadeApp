#include <cassert>
#include <iostream>

#include "Tetromino.h"
#include "Graphics/Screen.h"
#include "Graphics/Colour.h"
#include "Math/Vec2D.h"
#include "Utils/Utils.h"


Tetromino::Tetromino() : m_SignificantIndices(4, 0)
{
	m_Colour = Colour::Black();
	m_Type = TetrominoType::I_TYPE;
	m_XDirection = XBlockDirection::STOP;
	m_YDirection = YBlockDirection::STOP;
	m_IsMoving = true;
	m_Rotation = 0;

	// Fill mBlockData with empty rectangle
	AARectangle emptyRectangle = { Vec2D::Zero, Vec2D::Zero };
	for (size_t i = 0; i < 16; i++)
	{
		m_BlockData.push_back(emptyRectangle);
	}

};

Tetromino::Tetromino(const Tetromino& other)
{
	m_Colour = other.m_Colour;
	m_Type = other.m_Type;
	m_XDirection = other.m_XDirection;
	m_YDirection = other.m_YDirection;
	m_IsMoving = other.m_IsMoving;
	m_Rotation = other.m_Rotation;

	for (size_t i = 0; i < 16; i++)
	{
		m_BlockData.push_back(other.m_BlockData[i]);
	}

	for (size_t i = 0; i < 4; i++)
	{
		m_SignificantIndices.push_back(other.m_SignificantIndices[i]);
	}
}

Tetromino::~Tetromino()
{
}

void Tetromino::Init(TetrominoType type, const AARectangle& boundary, const Vec2D& startPosition)
{
	m_Type = type;
	m_Boundary = boundary;

	Vec2D yOffset = Vec2D::Zero;
	Vec2D xOffset = Vec2D::Zero;
	AARectangle block;

	for (size_t i = 0; i < 16; i++)
	{
		// Calculate the offset of each block
		if (i % 4 != 0)
		{
			xOffset += Vec2D(Tetromino::BLOCK_WIDTH, 0.0f);
		}
		else if (i != 0)
		{
			xOffset = Vec2D::Zero;
			yOffset += Vec2D(0.0f, Tetromino::BLOCK_HEIGHT);
		}

		m_BlockData[i] = { startPosition + xOffset + yOffset, Tetromino::BLOCK_WIDTH, Tetromino::BLOCK_HEIGHT };
	}

	// Set the indices for the specified bloack type
	SetInitialIndices();
}

void Tetromino::MoveBy(const Vec2D& vector)
{
	for (size_t i = 0; i < 16; i++)
	{
		m_BlockData[i].MoveBy(vector);
	}
}

void Tetromino::Rotate(int rotation)
{
	// Rotate the significant indices
	for (size_t i = 0; i < 4; i++)
	{
		m_SignificantIndices[i] = FindRotatedIndex(m_SignificantIndices[i], rotation);
	}
}

void Tetromino::Draw(Screen& theScreen)
{
	for (size_t i = 0; i < 4; i++)
	{
		unsigned int index = m_SignificantIndices[i];
		theScreen.Draw(m_BlockData[index], Colour::White(), true, m_Colour);
	}
}

// We will only run this if the tetromino can move to a possible location
void Tetromino::Update(uint32_t dt)
{
	MoveBy(GetPositionChange());
	Rotate(GetRotationChange());

	m_XDirection = XBlockDirection::STOP;
	m_YDirection = YBlockDirection::STOP;
	m_Rotation = 0;
}

void Tetromino::SetInitialIndices()
{
	// Set the array with the significant indices.
	switch (m_Type)
	{
	case TetrominoType::I_TYPE:
	{
		m_SignificantIndices = { 8, 9, 10, 11 };
		m_Colour = Colour::Cyan();
		break;
	}
	case TetrominoType::J_TYPE:
	{
		m_SignificantIndices = { 5, 6, 7, 11 };
		m_Colour = Colour::Blue();
		break;
	}
	case TetrominoType::L_TYPE:
	{
		m_SignificantIndices = { 5, 6, 7, 9 };
		m_Colour = Colour::Orange();
		break;
	}
	case TetrominoType::O_TYPE:
	{
		m_SignificantIndices = { 5, 6, 9, 10 };
		m_Colour = Colour::Yellow();
		break;
	}
	case TetrominoType::S_TYPE:
	{
		m_SignificantIndices = { 8, 9, 5, 6 };
		m_Colour = Colour::Green();
		break;
	}
	case TetrominoType::T_TYPE:
	{
		m_SignificantIndices = { 4, 5, 6, 9 };
		m_Colour = Colour::Pink();
		break;
	}
	case TetrominoType::Z_TYPE:
	{
		m_SignificantIndices = { 4, 5, 9, 10 };
		m_Colour = Colour::Red();
		break;
	}
	}
}

std::vector<AARectangle> Tetromino::GetSignificantBlocks() const
{
	std::vector<AARectangle> significantBlocks;

	int j = 0;

	for (const auto& i : m_SignificantIndices)
	{
		significantBlocks.push_back(m_BlockData[i]);
	}
	
	return significantBlocks;
}

int Tetromino::FindRotatedIndex(int index, int r)
{
	// transform the index into the x and y coordinates.
	int y = index / 4;
	int x = index - y * 4;

	switch (r % 4)
	{
	case 0: return y * 4 + x;			// 0 degrees
	case 1: return 12 + y - (x * 4);	// 90 degrees
	case 2: return 15 - (y * 4) - x;	// 180 degrees
	case 3: return 3 - y + (x * 4);		// 270 degrees
	}

	return 0;
}

Vec2D Tetromino::GetPositionChange() const
{
	Vec2D dir = Vec2D::Zero;
	if (m_XDirection != XBlockDirection::STOP)
	{
		if (m_XDirection == XBlockDirection::RIGHT)
		{
			dir += Vec2D(1.0f, 0.0f);
		}
		else
		{
			dir += Vec2D(-1.0f, 0.0f);
		}
	}

	if (m_YDirection != YBlockDirection::STOP)
	{
		if (m_YDirection == YBlockDirection::DOWN)
		{
			dir += Vec2D(0.0f, 1.0f);
		}
	}

	Vec2D dPos = dir * (float)BLOCK_HEIGHT;

	return dPos;
}

int Tetromino::GetRotationChange() const
{
	return m_Rotation;
}





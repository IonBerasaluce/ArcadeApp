#include "Block.h"
#include "Graphics/Screen.h"
#include "Ball.h"
#include "BoundaryEdge.h"


Block::Block(): m_OutlineColour(Colour::White()), m_FillColour(Colour::White()), m_Hp(1)
{

}

void Block::Init(const AARectangle& rect, int hp, const Colour& outlineColour, const Colour& fillColour)
{
	Excluder::Init(rect);
	m_Hp = hp;
	m_OutlineColour = outlineColour;
	m_FillColour = fillColour;
}

void Block::Draw(Screen& screen)
{
	screen.Draw(GetAARectangle(), m_OutlineColour, true, m_FillColour);
}

void Block::Bounce(Ball& ball, const BoundaryEdge& edge)
{
	ball.Bounce(edge);
}

void Block::ReduceHP()
{
	if (m_Hp > 0)
	{
		--m_Hp; 
	}
}

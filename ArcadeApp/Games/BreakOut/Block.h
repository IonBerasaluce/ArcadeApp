#pragma once

#include "Excluder.h"
#include "Graphics/Colour.h"

class Ball;
class Screen;
struct BoundaryEdge;

class Block : public Excluder
{
public:
	static const int UNBREAKABLE = -1;

	Block();
	void Init(const AARectangle& rect, int hp, const Colour& outlineColour, const Colour& fillColour);
	void Draw(Screen& screen);
	void Bounce(Ball& ball, const BoundaryEdge& edge);

	void ReduceHP();
	inline int GetHP() const { return m_Hp; }
	inline bool IsDestroyed() const { return m_Hp == 0; }
	inline const Colour& GetOutlineColour() const { return m_OutlineColour; }
	inline const Colour& GetFillColour() const { return m_FillColour; }

private:
	Colour m_OutlineColour;
	Colour m_FillColour;
	int m_Hp;
};


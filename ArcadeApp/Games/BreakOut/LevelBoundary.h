#include "Excluder.h"

class AARectangle;
class Ball;

class LevelBoundary
{
public:

	LevelBoundary(){}
	LevelBoundary(const AARectangle& rect);
	bool HasCollided(const Ball& ball, BoundaryEdge& edge);
	inline const AARectangle& GetAARectangle() const {return m_Includer.GetAARectangle();}

private:
	bool HasCollidedWithEdge(const Ball& ball, const BoundaryEdge& edge) const;//check to see if we collided at all
	Excluder m_Includer;
};
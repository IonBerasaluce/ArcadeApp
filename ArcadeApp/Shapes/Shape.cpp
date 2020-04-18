#include "Shape.h"

void Shape::MoveBy(const Vec2D& deltaOfsset)
{
	for (Vec2D& point : mPoints)
	{
		point = point + deltaOfsset;
	}
}

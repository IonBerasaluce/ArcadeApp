
#include <cassert>
#include <vector>

#include "Star2D.h"
#include "Line2D.h"
#include "Math/Vec2D.h"
#include "Utils/Utils.h"

Star2D::Star2D(const Star2D& inputStar)
{
	mLegs = inputStar.mLegs;

	bool res = Init(mLegs);
	assert(res);

	for (size_t i = 0; i < mLegs; i++)
	{
		ptrLines[i] = inputStar.ptrLines[i];
	}
}

Star2D::Star2D(Vec2D centre, float len, size_t legs)
{
	bool res = Init(legs);
	assert(res);

	ptrLines[0] = new Line2D(Vec2D(centre.GetX(), centre.GetY() - len/2), Vec2D(centre.GetX(), centre.GetY() + len/2));
	float alpha_deg = 360.0f / (2.0f * (float)legs);
	float alpha = toRad(alpha_deg);
	
	for (size_t i = 1; i < legs; i++)
	{
		Line2D newLine = ptrLines[i - 1]->rotateWithResult(alpha);
		ptrLines[i] = new Line2D(newLine.GetP0(), newLine.GetP1());
	}
}

Star2D::~Star2D()
{
	delete[] ptrLines;
}

bool Star2D::Init(size_t legs)
{
	mLegs = legs;
	ptrLines = new Line2D*[legs];

	if (!ptrLines)
	{
		return false;
	}

	return true;
}

Star2D Star2D::moveStar(Vec2D newCentre) const
{
	Star2D newStar(newCentre, mLength, mLegs);
	return newStar;
}

void Star2D::rotateStar(float angle)
{
	assert(ptrLines != nullptr);

	for (size_t i = 0; i < mLegs; i++)
	{
		ptrLines[i]->rotate(angle);
	}
}

Line2D& Star2D::operator[](const size_t index) const
{
	assert(ptrLines != nullptr);

	if (index >= mLegs)
	{
		std::cout << "Error index out of range!" << std::endl;
		assert(false);
	}
	
	return *ptrLines[index];
}

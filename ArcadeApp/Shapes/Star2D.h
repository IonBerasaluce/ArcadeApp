#pragma once
#include "Line2D.h"
#include "Math/Vec2D.h"
#include <vector>

class Star2D
{
public:
	Star2D():ptrLines(nullptr), mLength(0), mCentre(Vec2D(0, 0)), mLegs(0) {};
	Star2D(const Star2D& inputStar);
	Star2D(Vec2D centre, float len, size_t legs);
	~Star2D();

	inline size_t getLegs() const { return mLegs; }

	bool Init(size_t legs);

	Star2D moveStar(Vec2D newCentre) const;
	void rotateStar(float angle);

	Line2D& operator[](const size_t index) const;

private:
	Vec2D mCentre; 
	float mLength;
	size_t mLegs;
	Line2D** ptrLines;
};
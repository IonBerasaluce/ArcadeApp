#pragma once

#include "Math/Vec2D.h"
#include <vector>

class Shape
{
public:
	// Pure abstract classes don't usualy have a constructor since they have no data to initializer, however, they do have a destructor which is virtual
	virtual Vec2D GetCenterPoint() const = 0;
	virtual ~Shape() {}
	inline virtual std::vector<Vec2D> GetPoints() const { return mPoints; }
	void MoveBy(const Vec2D& deltaOffset);
	virtual void MoveTo(const Vec2D& position) = 0; 

//Anyone who inherits this class will have access to this data
protected:
	std::vector<Vec2D> mPoints;
};
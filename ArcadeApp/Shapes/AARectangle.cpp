#include "AARectangle.h"

#

AARectangle::AARectangle():AARectangle(Vec2D::Zero, Vec2D::Zero)
{
}

AARectangle::AARectangle(const Vec2D& topLeft, unsigned int width, unsigned int height)
{
	mPoints.push_back(topLeft);
	mPoints.push_back(Vec2D(topLeft.GetX() + width - 1, topLeft.GetY() + height -1));
	
}

AARectangle::AARectangle(const Vec2D& topLeft, const Vec2D& bottomRight)
{
	mPoints.push_back(topLeft);
	mPoints.push_back(bottomRight);
}

float AARectangle::GetWidth() const
{
	return GetBottomRight().GetX() - GetTopLeft().GetX() + 1;
}

float AARectangle::GetHeight() const
{
	return GetBottomRight().GetY() - GetTopLeft().GetY() + 1;
}

void AARectangle::MoveTo(const Vec2D& position)
{
	float width = GetWidth();
	float height = GetHeight();

	SetTopLeftPoint(position);
	SetBottomRight(Vec2D(position.GetX() + width - 1, position.GetY() + height - 1));
}

Vec2D AARectangle::GetCenterPoint() const
{
	return Vec2D((GetTopLeft().GetX() + GetBottomRight().GetX())/2.0f, (GetTopLeft().GetY() + GetBottomRight().GetY())/2.0f);
}

bool AARectangle::Intersects(const AARectangle& otherRect) const
{
	return !(otherRect.GetBottomRight().GetX() < GetTopLeft().GetX() ||
		otherRect.GetTopLeft().GetX() > GetBottomRight().GetX() ||
		otherRect.GetBottomRight().GetY() < GetTopLeft().GetY() ||
		otherRect.GetTopLeft().GetY() > GetBottomRight().GetY());
}

bool AARectangle::ContainsPoint(const Vec2D& p) const
{
	bool withinX = p.GetX() >= GetTopLeft().GetX() && p.GetX() <= GetBottomRight().GetX();
	bool withinY = p.GetY() >= GetTopLeft().GetY() && p.GetY() <= GetBottomRight().GetY();

	return withinX && withinY;
}

AARectangle AARectangle::Inset(const AARectangle& rect, Vec2D& insets)
{	
	return AARectangle(rect.GetTopLeft() + insets, rect.GetWidth() - 2 * insets.GetX(), rect.GetHeight() - 2 * insets.GetY());
}

void AARectangle::PrintCoordinates() const
{
	std::cout << "(" << GetTopLeft().GetX() << "," << GetTopLeft().GetY() << ") (" << GetBottomRight().GetX() << "," << GetBottomRight().GetY() << ")" << std::endl;
}

std::vector<Vec2D> AARectangle::GetPoints() const
{
	std::vector<Vec2D> points;

	points.push_back(mPoints[0]);
	points.push_back(Vec2D(mPoints[1].GetX(), mPoints[0].GetY()));
	points.push_back(mPoints[1]);
	points.push_back(Vec2D(mPoints[0].GetX(), mPoints[1].GetY()));

	return points;
}

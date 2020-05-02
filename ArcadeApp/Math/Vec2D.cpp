#include "Vec2D.h"
#include "Utils/Utils.h"

#include <cmath>
#include <cassert>

const Vec2D Vec2D::Zero(0, 0);

std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec)
{
	std::cout << "X: " << vec.mX << ", Y: " << vec.mY << std::endl;
	return consoleOut;
}

Vec2D operator*(float scale, const Vec2D& vec)
{
	return vec * scale;
}

bool Vec2D::operator==(const Vec2D& vec2) const
{
	return IsEqual(mX, vec2.mX) && IsEqual(mY, vec2.mY);
}

bool Vec2D::operator!=(const Vec2D& vec2) const
{
	return !(*this == vec2);
}

float Vec2D::Mag() const
{
	return sqrt(Mag2());
}

float Vec2D::Mag2() const
{
	return Dot(*this);
}

Vec2D Vec2D::operator-() const
{
	return Vec2D(-1 * mX, -1 * mY);
}

Vec2D Vec2D::operator*(float scale) const
{
	return Vec2D(scale *mX, scale * mY);
}

Vec2D Vec2D::operator/(float scale) const
{
	assert(fabsf(scale) > EPSILON);
	return Vec2D(mX/scale, mY/scale);
}

Vec2D Vec2D::operator+(const Vec2D& vec) const
{
	return Vec2D(mX + vec.mX, mY + vec.mY);
}

Vec2D Vec2D::operator-(const Vec2D &vec) const
{
	return Vec2D(mX - vec.mX, mY - vec.mY);
}

Vec2D& Vec2D::operator+=(const Vec2D& vec)
{
	*this = *this + vec;
	return *this;
}

Vec2D& Vec2D::operator-=(const Vec2D& vec)
{
	*this = *this - vec;
	return *this;
}

Vec2D Vec2D::GetUnitVec() const
{
	float magnitude = Mag();
	if (magnitude > EPSILON)
	{
		return *this / magnitude;
	}
	
	return Vec2D::Zero;
}

Vec2D& Vec2D::Normalize()
{
	*this = GetUnitVec();
	return *this;
}

float Vec2D::Distance(const Vec2D& vec) const
{
	return (*this - vec).Mag();
}

float Vec2D::Dot(const Vec2D& vec) const
{
	
	return (mX * vec.mX + mY * vec.mY);
}

float Vec2D::Det(const Vec2D& vec) const
{
	return (mX * vec.mY - mY * vec.mX);
}

Vec2D Vec2D::ProjectOnto(const Vec2D& vec) const
{
	Vec2D unitVec = vec.GetUnitVec();
	float dotProd = Dot(unitVec);
	return dotProd * vec;
}

float Vec2D::AngleBetween(const Vec2D& vec) const
{
	return acosf(GetUnitVec().Dot(vec.GetUnitVec()));
}

float Vec2D::AngleBetween360(const Vec2D& vec) const
{
	float dotProd = Dot(vec);
	float det = Det(vec);

	return atan2f(det, dotProd);
}

Vec2D Vec2D::Reflect(const Vec2D& normal) const
{
	return *this - 2 * ProjectOnto(normal);
}

void Vec2D::Rotate(float angle, const Vec2D& aroundPoint)
{
	float cosine = cosf(angle);
	float sine = sinf(angle);

	Vec2D thisVec(mX, mY);

	thisVec -= aroundPoint;

	float xRot = thisVec.mX * cosine - thisVec.mY * sine;
	float yRot = thisVec.mX * sine + thisVec.mY * cosine;

	Vec2D rot = Vec2D(xRot, yRot);

	*this = rot + aroundPoint;
}

Vec2D Vec2D::RotationResults(float angle, const Vec2D& aroundPoint) const
{
	float cosine = cosf(angle);
	float sine = sinf(angle);

	Vec2D thisVec(mX, mY);

	thisVec -= aroundPoint;

	float xRot = thisVec.mX * cosine - thisVec.mY * sine;
	float yRot = thisVec.mX * sine + thisVec.mY * cosine;

	Vec2D rot = Vec2D(xRot, yRot);

	return rot + aroundPoint;
}
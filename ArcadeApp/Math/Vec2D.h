#pragma once
#include <iostream>


class Vec2D
{
public:
	Vec2D() : Vec2D(0, 0) {}
	Vec2D(float x, float y) : mX(x), mY(y) {}

	static const Vec2D Zero;

	inline void SetX(float x) { mX = x; }
	inline void SetY(float y) { mY = y; }
	inline float GetX() const { return mX; }
	inline float GetY() const { return mY; }

	friend std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec);

	bool operator==(const Vec2D& vec2) const;
	bool operator!=(const Vec2D& vec2) const;

	float Mag() const;
	float Mag2() const;

	// Operators
	Vec2D operator-() const;
	Vec2D operator*(float scale) const;
	Vec2D operator/(float scale) const;

	Vec2D operator+(const Vec2D& vec) const;
	Vec2D operator-(const Vec2D& vec) const;
	Vec2D& operator+=(const Vec2D& vec);
	Vec2D& operator-=(const Vec2D& vec);

	// Methods
	Vec2D GetUnitVec() const;
	Vec2D& Normalize();
	Vec2D Normal() const;
	float Distance(const Vec2D& vec) const;
	float Dot(const Vec2D& vec) const;
	float Det(const Vec2D& vec) const;
	Vec2D ProjectOnto(const Vec2D& vec) const;
	float AngleBetween(const Vec2D& vec) const;
	float AngleBetween360(const Vec2D& vec) const;
	Vec2D Reflect(const Vec2D& normal) const;
	void Rotate(float angle, const Vec2D& aroundPoint);
	Vec2D RotationResults(float angle, const Vec2D& aroundPoint) const;


	// This fucntion makes 4 * vec work like vec * 4;
	friend Vec2D operator*(float scale, const Vec2D& vec);

private:
	float mX, mY;
};

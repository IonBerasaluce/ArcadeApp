#pragma once
#include <iostream>

class Fraction
{
public:
	Fraction();
	Fraction(int num, int denom);

	static const Fraction half;
	static const Fraction third;
	static const Fraction quarter;
	static const Fraction fifth;

	Fraction operator+(const Fraction b_frac) const;
	Fraction operator-(const Fraction b_frac) const;
	Fraction operator*(const Fraction b_frac) const;
	Fraction operator/(const Fraction b_frac) const;
	
	Fraction& operator+=(const Fraction b_frac);
	Fraction& operator-=(const Fraction b_frac);
	Fraction& operator*=(const Fraction b_frac);
	Fraction& operator/=(const Fraction b_frac);

	void Compare(Fraction a_frac, Fraction b_frac) const;
	
	inline int GetNumerator() const { return numerator; }
	inline int GetDenominator() const { return denominator; }

	friend std::ostream& operator<<(std::ostream& consoleOut, Fraction& a_frac);

private:
	int numerator;
	int denominator;
	float decimal;
	int divisor;
	void simplify();
};
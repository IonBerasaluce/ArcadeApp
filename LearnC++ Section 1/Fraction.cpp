#include "Fraction.h"
#include "Utils.h"


Fraction::Fraction(int num, int denom)
{
	numerator = num;
	denominator = denom;
	simplify();
}


int gcd(int a, int b)
{
	if (a == 0)
	{
		return b;
	}
	else
	{
		return gcd(b % a, a);
	}
}

Fraction::Fraction() :Fraction(0, 1)
{}

const Fraction Fraction::half(1, 2);
const Fraction Fraction::third(1, 3);
const Fraction Fraction::quarter(1, 4);
const Fraction Fraction::fifth(1, 5);

void Fraction::simplify()
{
	divisor = gcd(numerator, denominator);
	numerator /= divisor;
	denominator /= divisor;
}

Fraction Fraction::operator+(const Fraction b_frac) const
{
	return Fraction((numerator * b_frac.denominator) + (b_frac.numerator * denominator), (denominator * b_frac.denominator));
}

Fraction Fraction::operator-(const Fraction b_frac) const
{
	return Fraction((numerator * b_frac.denominator) - (b_frac.numerator - denominator), (denominator * b_frac.denominator));
}

Fraction Fraction::operator*(const Fraction b_frac) const
{
	return Fraction(numerator * b_frac.numerator, denominator * b_frac.denominator);
}

Fraction Fraction::operator/(const Fraction b_frac) const
{
	return Fraction(numerator * b_frac.denominator, denominator * b_frac.numerator);
}

Fraction& Fraction::operator+=(const Fraction b_frac)
{
	*this = *this + b_frac;
	return *this;
}

Fraction& Fraction::operator-=(const Fraction b_frac)
{
	*this = *this - b_frac;
	return *this;
}

Fraction& Fraction::operator*=(const Fraction b_frac)
{
	*this = *this * b_frac;
	return *this;
}

Fraction& Fraction::operator/=(const Fraction b_frac)
{
	*this = *this / b_frac;
	return *this;
}

void Fraction::Compare(Fraction a_frac, Fraction b_frac) const
{
	if (a_frac.decimal == b_frac.decimal)
	{
		std::cout << "The two fractions are equal! \n";
	}
	else if (a_frac.decimal > b_frac.decimal)
	{
		std::cout << a_frac << "is greater than: " << b_frac << "\n";
	}
	else
	{
		std::cout << b_frac << " is greater than: " << a_frac << "\n";
	}
}


std::ostream& operator<<(std::ostream& consoleOut, Fraction& a_frac)
{
	return consoleOut << a_frac.numerator << '/' << a_frac.denominator;
}
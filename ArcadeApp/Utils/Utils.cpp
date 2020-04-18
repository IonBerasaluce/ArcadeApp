#include <cmath>
#include "Utils.h"
#include <algorithm>
#include <cctype>

float MillisecondsToSeconds(unsigned int milliseconds)
{
	return static_cast<float> (milliseconds) / 1000.0;
}

bool IsEqual(float x, float y)
{
	return fabsf(x - y) < EPSILON;
}

bool IsGreaterThanOrEqual(float x, float y)
{
	return x > y || IsEqual(x, y);
}

bool IsLessThanOrEqual(float x, float y)
{
	return x < y || IsEqual(x, y);
}

float toRad(float angle_deg)
{
	return angle_deg * PI / 180;
}

unsigned int GetIndex(unsigned int width, unsigned int r, unsigned int c)
{
	return r * width + c;
}

bool StringCompare(const std::string& a, const std::string& b)
{
	if (a.length() == b.length())
	{
		return std::equal(b.begin(), b.end(), a.begin(), [](unsigned char a, unsigned char b) {
			return std::tolower(a) == std::tolower(b); });
	}
	else
	{
		return false;
	}
}

float Clamp(float val, float min, float max)
{
	if (val > max)
	{
		return max;
	}
	else if (val < min)
	{
		return min;
	}
	else
	{
		return val;
	}
}

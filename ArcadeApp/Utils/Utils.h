#pragma once
#include <string>

static const float EPSILON = 0.00001f;
static const float PI = 3.141592f;
static const float TWO_PI = PI * 2.0f;

float MillisecondsToSeconds(unsigned int milliseconds);

bool IsEqual(float x, float y);
bool IsGreaterThanOrEqual(float x, float y);
bool IsLessThanOrEqual(float x, float y);
float toRad(float angle_deg);

unsigned int GetIndex(unsigned int width, unsigned int r, unsigned int c);
bool StringCompare(const std::string& a, const std::string& b);

float Clamp(float val, float min, float max);

struct Size
{
	unsigned int width = 0, height = 0;
};
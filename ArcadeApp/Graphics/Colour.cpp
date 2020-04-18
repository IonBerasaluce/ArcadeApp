
#include "Colour.h"
#include "SDL.h"

const SDL_PixelFormat* Colour::mFormat = nullptr;

void Colour::InitColourFormat(const SDL_PixelFormat* format)
{
	Colour::mFormat = format;
}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	setRGBA(r, g, b, a);
}

void Colour::setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	mColour = SDL_MapRGBA(mFormat, r, g, b, a);
}

void Colour::setRed(uint8_t red)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);
	setRGBA(red, g, b, a);
}

void Colour::setGreen(uint8_t green)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);
	setRGBA(r, green, b, a);
}

void Colour::setBlue(uint8_t blue)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);
	setRGBA(r, g, blue, a);
}

void Colour::setAlpha(uint8_t alpha)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);
	setRGBA(r, g, b, alpha);
}

uint8_t Colour::GetRed() const
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);

	return r;
}

uint8_t Colour::GetGreen() const
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);

	return g;
}

uint8_t Colour::GetBlue() const
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);

	return b;
}

uint8_t Colour::GetAlpha() const
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColour, mFormat, &r, &g, &b, &a);

	return a;
}

Colour Colour::EvaluateMinuSourceAlpha(const Colour& source, const Colour& destination)
{
	uint8_t alpha = source.GetAlpha();

	float sourceAlpha = float(alpha) / 255.0f;
	float destAlpha = 1.0f - sourceAlpha;

	Colour outColour;
	outColour.setAlpha(255);
	outColour.setRed(float(source.GetRed()) * sourceAlpha + destination.GetRed() * destAlpha);
	outColour.setGreen(float(source.GetGreen()) * sourceAlpha + destination.GetGreen() * destAlpha);
	outColour.setBlue(float(source.GetBlue()) * sourceAlpha + destination.GetBlue() * destAlpha);

	return outColour;
}

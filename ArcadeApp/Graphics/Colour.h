#pragma once

#include <stdint.h>

struct SDL_PixelFormat;

class Colour
{

public:

	static const SDL_PixelFormat* mFormat;
	static void InitColourFormat(const SDL_PixelFormat* format);

	//Blending equation here
	//Source RGB * source Alpha + DEstinationRGB * (1 - source Alpha)
	static Colour EvaluateMinuSourceAlpha(const Colour& source, const Colour& destination);


	Colour():mColour(0){}
	Colour(uint32_t colour): mColour(colour){}
	Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	bool operator==(const Colour& c) const { return mColour == c.mColour;  }
	bool operator!=(const Colour& c) const { return !(*this == c.mColour); }
	inline uint32_t GetPixelColour() const { return mColour; }

	void setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void setRed(uint8_t red);
	void setGreen(uint8_t green);
	void setBlue(uint8_t blue);
	void setAlpha(uint8_t alpha);

	uint8_t GetRed() const;
	uint8_t GetBlue() const;
	uint8_t GetGreen() const;
	uint8_t GetAlpha() const;

	// Default colours ready to use
	static Colour Black() { return Colour(0, 0, 0, 255); }
	static Colour White() { return Colour(255, 255, 255, 255); }
	static Colour Red() { return Colour(255, 0, 0, 255); }
	static Colour Green() { return Colour(0, 255, 0, 255); }
	static Colour Blue() { return Colour(0, 0, 255, 255); }

	static Colour Yellow() { return Colour(255, 255, 0, 255); }
	static Colour Magenta() { return Colour(255, 0, 255, 255); }
	static Colour Cyan() { return Colour(37, 240, 217, 255); }
	static Colour Pink() { return Colour(252, 197, 224, 255); }
	static Colour Orange() { return Colour(245, 190, 100, 255); }

private:
	uint32_t mColour;

};
#pragma once

#include <stdint.h>
#include "Colour.h"

struct SDL_Surface;

class ScreenBuffer
{
public:
	ScreenBuffer();
	ScreenBuffer(const ScreenBuffer& screenBuffer);
	~ScreenBuffer();

	ScreenBuffer& operator=(const ScreenBuffer& screenBuffer);

	void Init(uint32_t format, uint32_t width, uint32_t height);

	inline SDL_Surface* GetSurface() { return mSurface; }

	void Clear(const Colour& c = Colour::Black());

	void SetPixel(const Colour& colour, int x, int y);

private:
	SDL_Surface* mSurface;
	uint32_t GetIndex(int r, int c);
};
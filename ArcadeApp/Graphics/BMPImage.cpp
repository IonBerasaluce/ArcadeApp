#include "BMPImage.h"
#include "SDL.h"

BMPImage::BMPImage():m_Width (0), m_Height(0)
{
}

bool BMPImage::Load(const std::string& path)
{
	SDL_Surface* bmpSurface = SDL_LoadBMP(path.c_str());

	if (bmpSurface == nullptr)
	{
		return false;
	}
	
	m_Width = bmpSurface->w;
	m_Height = bmpSurface->h;

	uint32_t lengthOfFile = m_Width * m_Height;

	m_Pixels.reserve(lengthOfFile);
	uint32_t* pixels = static_cast<uint32_t*>(bmpSurface->pixels);

	for (uint32_t i = 0; i < lengthOfFile; i++)
	{
		m_Pixels.push_back(Colour(pixels[i]));
	}

	SDL_LockSurface(bmpSurface);
	SDL_FreeSurface(bmpSurface);

	return true;
}

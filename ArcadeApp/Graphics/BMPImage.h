#pragma once


#include "Colour.h"
#include <vector>
#include <string>
#include <stdint.h>

class BMPImage
{
public:
	BMPImage();
	bool Load(const std::string& path);

	inline const std::vector<Colour>& GetPixels() const { return m_Pixels; }
	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }

private:
	std::vector<Colour> m_Pixels;
	uint32_t m_Width;
	uint32_t m_Height;
};
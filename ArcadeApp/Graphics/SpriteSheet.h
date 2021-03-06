#pragma once

#include "BMPImage.h"
#include "Shapes/AARectangle.h"
#include <string>
#include <vector>


struct Sprite
{
	uint32_t xPos = 0;
	uint32_t yPos = 0;
	uint32_t width = 0; 
	uint32_t height = 0;

	Vec2D Offset()
	{
		AARectangle rect = AARectangle(Vec2D((float)xPos, (float)yPos), width, height);
		Vec2D offset = rect.GetTopLeft() - rect.GetCenterPoint();
		return offset;
	}
};

class SpriteSheet
{
public:
	SpriteSheet();
	bool Load(const std::string& name);
	Sprite GetSprite(const std::string& spriteName) const;

	std::vector<std::string> SpriteNames() const;

	inline const BMPImage& GetBMPImage() const { return m_BMPImage; }
	uint32_t GetHeight() const { return m_BMPImage.GetHeight(); }
	uint32_t GetWidth() const { return m_BMPImage.GetWidth(); }


private:
	bool LoadSpriteSections(const std::string& path);

	struct BMPImageSection
	{
		std::string key = "";
		Sprite sprite;
	};

	BMPImage m_BMPImage;
	std::vector<BMPImageSection> m_Sections;
};
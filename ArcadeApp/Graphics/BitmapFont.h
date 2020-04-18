#pragma once

#include <stdint.h>
#include "SpriteSheet.h"
#include "Utils/Utils.h"

class Vec2D;
class AARectangle;

enum class BitmapFontXAlignment 
{
	BFXA_LEFT = 0,
	BFXA_CENTRE,
	BFXA_RIGHT
};

enum class BitmapFontYAlignment
{
	BFYA_TOP = 0,
	BFYA_CENTRE,
	BFYA_BOTTOM
};

class BitmapFont
{
public:
	BitmapFont(){}
	bool Load(const std::string& name);
	Size GetSizeOf(const std::string& str) const;
	Vec2D GetDrawPosition(const std::string& str, const AARectangle& box, BitmapFontXAlignment xAlign = BitmapFontXAlignment::BFXA_LEFT, BitmapFontYAlignment yAlign = BitmapFontYAlignment::BFYA_TOP) const;
	
	inline const SpriteSheet& GetSpriteSheet() const { return m_FontSheet; }
	inline const uint32_t GetFontSpacingBetweenLetters() const { return 2; }
	inline const uint32_t GetFontSpacingBetweenWords() const { return 5; }

private:
	SpriteSheet m_FontSheet;
};
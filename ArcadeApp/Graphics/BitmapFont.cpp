#include "BitmapFont.h"
#include "Shapes/AARectangle.h"
#include "Math/Vec2D.h"

bool BitmapFont::Load(const std::string& name)
{
	return m_FontSheet.Load(name);
}

Size BitmapFont::GetSizeOf(const std::string& str) const
{
	// How long and high in pixels the sheet is
	Size textSize;
	size_t length = str.length();

	int i = 0;

	for (char c : str)
	{
		if (c == ' ')
		{
			textSize.width += GetFontSpacingBetweenWords();
			++i;
			continue;
		}

		Sprite sprite = m_FontSheet.GetSprite(std::string("") + c);
		textSize.height = textSize.height < sprite.height ? sprite.height : textSize.height;
		textSize.width += sprite.width;

		if (i + 1 < length)
		{
			textSize.width += GetFontSpacingBetweenLetters();
		}

		++i;
	}

	return textSize;
}

Vec2D BitmapFont::GetDrawPosition(const std::string& str, const AARectangle& box, BitmapFontXAlignment xAlign, BitmapFontYAlignment yAlign) const
{
	Size textSize = GetSizeOf(str);
	
	// Claculate where to draw the string based on the size we have
	uint32_t x = 0;
	uint32_t y = 0;

	if (xAlign == BitmapFontXAlignment::BFXA_CENTRE)
	{
		x = (uint32_t)(box.GetWidth() / 2 - textSize.width / 2);
	}
	else if (xAlign == BitmapFontXAlignment::BFXA_RIGHT)
	{
		x = (uint32_t) (box.GetWidth() - textSize.width);
	}
	else
	{
		x += (uint32_t)box.GetTopLeft().GetX();
	}

	x += (uint32_t)box.GetTopLeft().GetX();

	if (yAlign == BitmapFontYAlignment::BFYA_CENTRE)
	{
		y = (uint32_t)(box.GetHeight() / 2 - textSize.height / 2);
	}
	else if (yAlign == BitmapFontYAlignment::BFYA_BOTTOM)
	{
		y = (uint32_t)(box.GetHeight() - textSize.height);
	}
	else
	{
		y += (uint32_t)box.GetTopLeft().GetY();
	}

	y += (uint32_t)box.GetTopLeft().GetY();

	return Vec2D((float)x, (float)y);
}

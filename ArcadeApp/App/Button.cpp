#include "Button.h"
#include "Graphics/Screen.h"

Button::Button(const BitmapFont& bitmapFont, const Colour& textColour, const Colour& highlightColour):m_BitmapFont(bitmapFont), m_TextColour(textColour), m_HighlightColour(highlightColour), m_Title(""), m_Highlighted(false)
{
}

void Button::Init(Vec2D topLeft, unsigned int width, unsigned int height)
{
	m_BBox = AARectangle(topLeft, width, height);
}

void Button::Draw(Screen& theScreen)
{
	theScreen.Draw(m_BitmapFont, m_Title, m_BitmapFont.GetDrawPosition(m_Title, m_BBox, BitmapFontXAlignment::BFXA_CENTRE, BitmapFontYAlignment::BFYA_CENTRE), m_TextColour);

	if (m_Highlighted)
	{
		theScreen.Draw(m_BBox, m_HighlightColour);
	}
}

void Button::ExecuteAction()
{
	m_Action();
}

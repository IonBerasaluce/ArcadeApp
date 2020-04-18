#pragma once

#include <string>
#include <functional>

#include "Shapes/AARectangle.h"
#include "Graphics/BitmapFont.h"
#include "Graphics/Colour.h"
#include "Inputs/InputAction.h"

class Screen;

class Button
{
public:
	using ButtonAction = std::function<void(void)>;

	Button(const BitmapFont& bitmapFont, const Colour& textColour, const Colour& highlightColour = Colour::White());
	void Init(Vec2D topLeft, unsigned int width, unsigned int height);
	void Draw(Screen& theScreen);
	void ExecuteAction();

	inline void SetButtonText(const std::string& text) { m_Title = text; }
	inline const std::string& GetButtonText() const { return m_Title; }
	inline void SetHighlighted(bool highlighted) { m_Highlighted = highlighted; }
	inline bool IsHighlighted() const { return m_Highlighted; }
	inline void SetButtonAction(Button::ButtonAction action) { m_Action = action; }

private:
	const BitmapFont& m_BitmapFont;
	std::string m_Title;
	AARectangle m_BBox;

	ButtonAction m_Action;

	bool m_Highlighted;
	Colour m_HighlightColour;
	Colour m_TextColour;
};
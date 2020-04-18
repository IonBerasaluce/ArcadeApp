#pragma once

#include <stdint.h>
#include <vector>
#include <functional>

#include "Graphics/ScreenBuffer.h"
#include "Graphics/Colour.h"

class Vec2D;
class Line2D;
class Star2D;
class Triangle;
class AARectangle;
class Circle;
class BMPImage;
class SpriteSheet;
class BitmapFont;

struct Sprite;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;
struct SDL_Surface;

class Screen
{
public:
	Screen();
	~Screen();

	SDL_Window* Init(uint32_t w, uint32_t h, uint32_t mag, bool fast = true);
	void SwapScreens();

	inline void SetClearColor(const Colour& clearColour) { m_ClearColour = clearColour; }
	inline uint32_t Width() const { return mWidth; }
	inline uint32_t Height() const { return mHeight; }
	
	// Draw methods go here.
	void Draw(int x, int y, const Colour& colour);
	void Draw(const Vec2D& vec, const Colour& colour);
	void Draw(const Line2D& line, const Colour& colour);
	void Draw(const Star2D& star, const Colour& colour);
	void Draw(const Triangle& triangle, const Colour& colour, bool fill = false, const Colour& fillColour = Colour::White());
	void Draw(const AARectangle& rectangle, const Colour& colour, bool fill = false, const Colour& fillColour = Colour::White());
	void Draw(const Circle& circle, const Colour& colour, bool fill = false, const Colour& fillColour = Colour::White());

	void Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, const Colour& overlayColour = Colour::White());
	void Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& position, const Colour& overlayColour = Colour::White());
	void Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& position, const Colour& overlayColour = Colour::White());


private:

	// This is not the preferred way of avoiding people  using = or copy constructor should this be avoided
	Screen(const Screen& screen);

	using FillPolyFunc = std::function<Colour(uint32_t x, uint32_t y)>;

	void ClearScreen();
	void FillPoly(const std::vector<Vec2D>& points, FillPolyFunc func);

	uint32_t mWidth;
	uint32_t mHeight;

	Colour m_ClearColour;
	ScreenBuffer m_BackBuffer;

	SDL_Window* ptrWindow;
	SDL_Surface* ptrWindowSurface;

	SDL_Renderer* m_Renderer;
	SDL_PixelFormat* m_PixelFormat;
	SDL_Texture* m_Texture;
	bool m_Fast;
};
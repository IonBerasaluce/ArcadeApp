#include <cassert>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "Graphics/Screen.h"
#include "SDL.h"
#include "Math/Vec2D.h"
#include "Utils/Utils.h"

#include "Shapes/Line2D.h"
#include "Shapes/Star2D.h"
#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"

#include "BMPImage.h"
#include "SpriteSheet.h"
#include "BitmapFont.h"


Screen::Screen():mWidth(0), mHeight(0), ptrWindow(nullptr), ptrWindowSurface(nullptr), m_Texture(nullptr), m_Renderer(nullptr), m_PixelFormat(nullptr), m_Fast(true)
{}

Screen::~Screen()
{
	if (m_PixelFormat)
	{
		SDL_FreeFormat(m_PixelFormat);
		m_PixelFormat = nullptr;\
	}

	if (m_Texture)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = nullptr;
	}

	if (m_Renderer)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

	if (ptrWindow)
	{
		SDL_DestroyWindow(ptrWindow);
		ptrWindow = nullptr;
	}
	SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t w, uint32_t h, uint32_t mag, bool fast)
{
	m_Fast = fast;

	if (SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "There is an Error initializing SDL. Error " << SDL_GetError() << std::endl;
		return nullptr;
	}

	mWidth = w;
	mHeight = h;

	ptrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth * mag, mHeight * mag, 0);

	if (ptrWindow)
	{
		uint8_t rClear = 0;
		uint8_t gClear = 0;
		uint8_t bClear = 0;
		uint8_t aClear = 255;

		if (m_Fast)
		{
			m_Renderer = SDL_CreateRenderer(ptrWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (m_Renderer == nullptr)
			{
				std::cout << "SDL_CreateRenderer failed!" << std::endl;
				return nullptr;
			}

			SDL_SetRenderDrawColor(m_Renderer, rClear, gClear, bClear, aClear);
		}
		else
		{
			ptrWindowSurface = SDL_GetWindowSurface(ptrWindow);
		}

		m_PixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);

		if (m_Fast)
		{
			m_Texture = SDL_CreateTexture(m_Renderer, m_PixelFormat->format, SDL_TEXTUREACCESS_STREAMING, w, h);
		}

		Colour::InitColourFormat(m_PixelFormat);

		m_ClearColour = Colour(rClear, gClear, bClear, aClear);

		m_BackBuffer.Init(m_PixelFormat->format, mWidth, mHeight);

		m_BackBuffer.Clear(m_ClearColour);
	}
	return ptrWindow;
}

void Screen::SwapScreens()
{
	assert(ptrWindow);
	if (ptrWindow)
	{
		ClearScreen();


		if (m_Fast)
		{
			uint8_t* textureData = nullptr;
			int texturePitch = 0;

			if (SDL_LockTexture(m_Texture, nullptr, (void**)&textureData, &texturePitch) >= 0)
			{
				SDL_Surface* surface = m_BackBuffer.GetSurface();

				// Lock ztexture returns a pointer to the texture which we then copy into the backbuffer which then goes into the screen
				memcpy(textureData, surface->pixels, surface->w * surface->h * m_PixelFormat->BytesPerPixel);

				SDL_UnlockTexture(m_Texture);

				SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);

				SDL_RenderPresent(m_Renderer);
			}
		}
		else
		{
			SDL_BlitScaled(m_BackBuffer.GetSurface(), nullptr, ptrWindowSurface, nullptr);

			SDL_UpdateWindowSurface(ptrWindow);
		}
		
		m_BackBuffer.Clear(m_ClearColour);
	}
}

void Screen::Draw(int x, int y, const Colour& colour)
{
	assert(ptrWindow);
	if (ptrWindow) 
	{
		m_BackBuffer.SetPixel(colour, x, y);
	}
}

void Screen::Draw(const Vec2D& vec, const Colour& colour)
{
	assert(ptrWindow);
	if (ptrWindow)
	{
		m_BackBuffer.SetPixel(colour, (int)vec.GetX(), (int)vec.GetY());
	}

}

void Screen::Draw(const Line2D& line, const Colour& colour)
{
	assert(ptrWindow);
	if (ptrWindow)
	{
		int dx, dy;

		int x0 = (int)(roundf(line.GetP0().GetX()));
		int y0 = (int)(roundf(line.GetP0().GetY()));
		int x1 = (int)(roundf(line.GetP1().GetX()));
		int y1 = (int)(roundf(line.GetP1().GetY()));

		dx = x1 - x0;
		dy = y1 - y0;

		signed const char ix((dx > 0) - (dx < 0)); //evaluate to 1 or -1
		signed const char iy((dy > 0) - (dy < 0));

		dx = abs(dx) * 2;
		dy = abs(dy) * 2;

		Draw(x0, y0, colour);

		if (dx >= dy)
		{
			// go along in the x
			int d = dy - dx / 2;
			while (x0 != x1)
			{
				if (d >= 0)
				{
					d -= dx;
					y0 += iy;
				}

				d += dy;
				x0 += ix;

				Draw(x0, y0, colour);
			}
		}
		else
		{
			//go along the y
			int d = dx - dy / 2; 
			while (y0 != y1)
			{
				if (d >= 0)
				{
					d -= dy;
					x0 += ix;
				}

				d += dx;
				y0 += iy;

				Draw(x0, y0, colour);
			}
		}
	}
}

void Screen::Draw(const Star2D& star, const Colour& colour)
{
	for (size_t i = 0; i < star.getLegs(); i++)
	{
		Draw(star[i], colour);
	}
}

void Screen::Draw(const Triangle& triangle, const Colour& colour, bool fill, const Colour& fillColour)
{
	if (fill)
	{
		FillPoly(triangle.GetPoints(), [fillColour](uint32_t x, uint32_t y) { return fillColour; });
	}
	
	Line2D p0p1 = Line2D(triangle.GetP0(), triangle.GetP1());
	Line2D p1p2 = Line2D(triangle.GetP1(), triangle.GetP2());
	Line2D p2p0 = Line2D(triangle.GetP2(), triangle.GetP0());

	Draw(p0p1, colour);
	Draw(p1p2, colour);
	Draw(p2p0, colour);
}

void Screen::Draw(const AARectangle& rectangle, const Colour& colour, bool fill, const Colour& fillColour)
{
	if (fill)
	{
		FillPoly(rectangle.GetPoints(), [fillColour](uint32_t x, uint32_t y) { return fillColour; });
	}

	std::vector<Vec2D> points = rectangle.GetPoints();
	Line2D p0p1 = Line2D(points[0], points[1]);
	Line2D p1p2 = Line2D(points[1], points[2]);
	Line2D p2p3 = Line2D(points[2], points[3]);
	Line2D p3p0 = Line2D(points[3], points[0]);

	Draw(p0p1, colour);
	Draw(p1p2, colour);
	Draw(p2p3, colour);
	Draw(p3p0, colour);

}

void Screen::Draw(const Circle& circle, const Colour& colour, bool fill, const Colour& fillColour)
{
	static unsigned int NUM_CIRCLES_SEGMENTS = 30;
	float angle = TWO_PI / float(NUM_CIRCLES_SEGMENTS);

	std::vector<Vec2D> circlePoints;
	std::vector<Line2D> lines;

	Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
	Vec2D p1 = p0;
	Line2D nextLineToDraw;

	for (unsigned int i = 0; i < NUM_CIRCLES_SEGMENTS; ++i)
	{
		p1.Rotate(angle, circle.GetCenterPoint());
		nextLineToDraw.SetP1(p1);
		nextLineToDraw.SetP0(p0);

		lines.push_back(nextLineToDraw);
		circlePoints.push_back(p0);

		if (fill)
		{
			FillPoly(circlePoints, [fillColour](uint32_t x, uint32_t y) { return fillColour; });
		}
		
		for (const Line2D& line : lines)
		{
			Draw(line, colour);
		}
		p0 = p1;
	}
}


void Screen::Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, const Colour& overlayColour, float rotation)
{
	
	float rVal = static_cast<float>(overlayColour.GetRed()) / 255.0f;
	float gVal = static_cast<float>(overlayColour.GetGreen()) / 255.0f;
	float bVal = static_cast<float>(overlayColour.GetBlue()) / 255.0f;
	float aVal = static_cast<float>(overlayColour.GetAlpha()) / 255.0f;

	uint32_t width = sprite.width;
	uint32_t height = sprite.height;

	const std::vector<Colour>& pixels = image.GetPixels();

	// This is the position of where we will place the sprite on the screen
	Vec2D topLeft = pos;
	Vec2D topRight = pos + Vec2D((float)width, 0);
	Vec2D bottomLeft = pos + Vec2D(0, (float)height);
	Vec2D bottomRight = pos + Vec2D((float)width, (float)height);

	std::vector<Vec2D> points = { topLeft, bottomLeft, bottomRight, topRight };

	Vec2D xAxis = topRight - topLeft;
	Vec2D yAxis = bottomLeft - topLeft;

	// Less than one since we divide by the sprite width squared
	const float invXAxisLengthSq = 1.0f / xAxis.Mag2();
	const float invYAxisLengthSq = 1.0f / yAxis.Mag2();

	FillPoly(points, [&](uint32_t px, uint32_t py)
	{
			Vec2D p = { static_cast<float>(px), static_cast<float>(py) };
			Vec2D d = p - topLeft;

			// Value from 0 to 1 from where we are drawing in the screen no matter if the object is rotated or scaled
			float u = invXAxisLengthSq * d.Dot(xAxis);
			float v = invYAxisLengthSq * d.Dot(yAxis);

			u = Clamp(u, 0.0f, 1.0f);
			v = Clamp(v, 0.0f, 1.0f);

			float tx = roundf(u * static_cast<float>(sprite.width));
			float ty = roundf(v * static_cast<float>(sprite.height));

			Colour c = pixels[GetIndex(image.GetWidth(), (unsigned int)(ty + sprite.yPos), (unsigned int)(tx + sprite.xPos))];

			Colour nc = { static_cast<uint8_t>(c.GetRed() * rVal), static_cast<uint8_t>(c.GetGreen() * gVal), static_cast<uint8_t>(c.GetBlue() * bVal), static_cast<uint8_t>(c.GetAlpha() * aVal) };

			return nc;
	}, rotation);
}

void Screen::Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& position, const Colour& overlayColour, const float rotation)
{
	Draw(ss.GetBMPImage(), ss.GetSprite(spriteName), position, overlayColour, rotation);
}

void Screen::Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& position, const Colour& overlayColour)
{
	uint32_t xPos = (uint32_t)position.GetX();
	const SpriteSheet& ss = font.GetSpriteSheet();

	for (char c : textLine) 
	{
		if (c == ' ') 
		{
			xPos += font.GetFontSpacingBetweenWords();
			continue;
		}

		Sprite sprite = ss.GetSprite(std::string("") + c);

		Draw(ss.GetBMPImage(), sprite, Vec2D((float)xPos, (float)position.GetY()), overlayColour);

		xPos += sprite.width;

		xPos += font.GetFontSpacingBetweenLetters();
	}
}

void Screen::ClearScreen()
{
	assert(ptrWindow);
	if (ptrWindow)
	{
		if (m_Fast)
		{
			SDL_RenderClear(m_Renderer);
		}
		else
		{
			SDL_FillRect(ptrWindowSurface, nullptr, m_ClearColour.GetPixelColour());
		}
	}

}

void Screen::FillPoly(const std::vector<Vec2D>& points, FillPolyFunc func, float angle)
{
	if (points.size() > 0)
	{
		// Get the boundary from the pixel points
		float top = points[0].GetY();
		float bottom = points[0].GetY();
		float right = points[0].GetX();
		float left = points[0].GetX();

		for (size_t i = 1; i < points.size(); ++i)
		{
			if (points[i].GetY() < top)
			{
				top = points[i].GetY();
			}
			if (points[i].GetY() > bottom)
			{
				bottom = points[i].GetY();
			}
			if (points[i].GetX() < left)
			{
				left = points[i].GetX();
			}
			if (points[i].GetX() > right)
			{
				right = points[i].GetX();
			}
			
		}

		// We need to get the centre point for the rotation here and save the values for sin and cos of rotation angle
		Vec2D centerPoint = Vec2D((right - left) / 2, (bottom - top) / 2) + Vec2D(left, top);
		float cosine = cosf(angle);
		float sine = sinf(angle);

		// Scan line fill poly algorithm
		for (int pixelY = (int)top; pixelY < (int)bottom; ++pixelY)
		{
			// These are the x-intercepts
			std::vector<float> nodeXVec;
			size_t j = points.size() - 1;


			for (size_t i = 0; i < points.size(); i++)
			{
				float pointiY = points[i].GetY();
				float pointjY = points[j].GetY();
				
				// Finding the intercepts of the line with the borders of the shape. Points are ordered in Topleft, bottomLeft, bottomRight and topright
				if ((pointiY <= (float)pixelY && pointjY > (float)pixelY) || (pointjY <= (float)pixelY && pointiY > (float)pixelY))
				{
					float den = pointjY - pointiY;
					if (IsEqual(den, 0))
					{
						continue;
					}

					float x = points[i].GetX() + (pixelY - pointiY) / den * (points[j].GetX() - points[i].GetX());
					nodeXVec.push_back(x);
				}

				j = i;
			}
			std::sort(nodeXVec.begin(), nodeXVec.end(), std::less<>());
			
			for (size_t k = 0; k < nodeXVec.size(); k += 2)
			{
				if (nodeXVec[k + 1] > left)
				{
					if (nodeXVec[k] < left)
					{
						nodeXVec[k] = left;
					}
					if (nodeXVec[k + 1] > right)
					{
						nodeXVec[k + 1] = right;
					}
					
					for (int pixelX = (int)nodeXVec[k]; pixelX < nodeXVec[k + 1]; ++pixelX)
					{
						// Here the first 2 arguments have to be the rotated pixel locations and func(pixelX, pixelY) has to receive the non-rotated pixels
						// Update the position of the pixelX and Pixel Y based on the rotated pixel
						Vec2D rotatedPixel = Vec2D((float)pixelX, (float)pixelY);
						rotatedPixel.Rotate(angle, centerPoint);
						Draw((int)rotatedPixel.GetX(), (int)rotatedPixel.GetY(), func(pixelX, pixelY));
					}
				}
			}
		}
	}
}


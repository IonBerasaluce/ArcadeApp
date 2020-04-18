#include <iostream>
#include "SDL.h"
#include "Graphics/Colour.h"
#include "Graphics/ScreenBuffer.h"
#include "Graphics/Screen.h"
#include "Shapes/Line2D.h"
#include "App/App.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 2;

int main(int argc, char* argv[])
{
	// As long as we initialize correctly we can run
	if (App::Singleton().Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION))
	{
		App::Singleton().Run();
	}

	return 0;
}
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <SDL.h>

#include "main.hpp"

#include "Keyboard.hpp"
#include "Maze.hpp"

static const int NumDotsWidth = 56 * 3; 
static const int NumDotsHeight = 28 * 3;

static const int PixelsPerDot = 3;
static const int WindowWidth = NumDotsWidth * PixelsPerDot;
static const int WindowHeight = NumDotsHeight * PixelsPerDot;

static const int TexturePixelFormat = SDL_PIXELFORMAT_ABGR8888;
static const int BytesPerTexturePixel = 4; // must match TEXTURE_PIXEL_FORMAT
static const int FrameBufferPitch = NumDotsWidth * BytesPerTexturePixel;

static const int RendersPerSecond = 30;
static const Uint32 MillisecondsPerRender = 1000 / RendersPerSecond;

struct WindowData
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

struct SystemEventData
{
	bool quit = false;
	Keyboard keyboard;
};

static bool setup(int windowWidth, int windowHeight, WindowData& windowData)
{
	bool success = false;

	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		windowData.window = SDL_CreateWindow(
			WindowTitle,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			windowWidth, windowHeight,
			SDL_WINDOW_SHOWN);
		if (windowData.window != nullptr)
		{
			windowData.renderer = SDL_CreateRenderer(windowData.window, -1, SDL_RENDERER_ACCELERATED);
			if (windowData.renderer != nullptr)
			{
				windowData.texture = SDL_CreateTexture(
					windowData.renderer,
					TexturePixelFormat,
					SDL_TEXTUREACCESS_STREAMING,
					NumDotsWidth, NumDotsHeight);
				if (windowData.texture != nullptr)
				{
					success = true;
				}
				else
				{
					std::cerr << "Could not create texture: " << SDL_GetError() << std::endl;
				}
			}
			else
			{
				std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
			}
		}
		else
		{
			std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
		}
	}
	else
	{
		std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
	}

	return success;
}

static void shutdown(WindowData& windowData)
{
	SDL_DestroyTexture(windowData.texture);
	windowData.texture = nullptr;

	SDL_DestroyRenderer(windowData.renderer);
	windowData.renderer = nullptr;

	SDL_DestroyWindow(windowData.window);
	windowData.window = nullptr;

	SDL_Quit();
}

static void processEvents(SystemEventData& systemEventData)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			systemEventData.quit = true;
		}
		else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		{
			Key key;

			switch (event.key.keysym.sym)
			{
			default:
				return; // unused key

			case SDLK_ESCAPE:
				systemEventData.quit = true;
				return;

			case SDLK_UP:
				key = Key::UpArrow;
				break;
			case SDLK_DOWN:
				key = Key::DownArrow;
				break;
			case SDLK_LEFT:
				key = Key::LeftArrow;
				break;
			case SDLK_RIGHT:
				key = Key::RightArrow;
				break;
			}

			if (event.key.type == SDL_KEYDOWN)
			{
				systemEventData.keyboard.press(key);
			}
			else
			{
				systemEventData.keyboard.release(key);
			}
		}
	}
}

static void render(WindowData& windowData, const std::vector<Color>& colorBuffer, std::vector<Uint8>& frameBuffer)
{
	const Color* color = colorBuffer.data();
	Uint8* row = frameBuffer.data();
	for (int y = 0; y < NumDotsHeight; ++y)
	{
		Uint32* pixel = reinterpret_cast<Uint32*>(row);
		for (int x = 0; x < NumDotsWidth; ++x)
		{
			Uint32 output = 0;

			switch (*color)
			{
			case Color::Black:
				output = 0;
				break;
			case Color::White:
				output = 0xFFFFFFFF;
				break;
			case Color::VerticalStripe:
				output = (x % 2 == 0) ? 0 : 0xFFFFFFFF;
				break;
			case Color::HorizontalStripe:
				output = (y % 2 == 0) ? 0 : 0xFFFFFFFF;
				break;
			case Color::Alternating:
				if (x % 2 == 0)
				{
					output = (y % 2 == 0) ? 0 : 0xFFFFFFFF;
				}
				else
				{
					output = (y % 2 != 0) ? 0 : 0xFFFFFFFF;
				}
				break;
			}

			++color;
			*pixel++ = output;
		}
		row += FrameBufferPitch;
	}

	SDL_UpdateTexture(windowData.texture, NULL, frameBuffer.data(), FrameBufferPitch);
	SDL_RenderCopy(windowData.renderer, windowData.texture, NULL, NULL);
	SDL_RenderPresent(windowData.renderer);
}

int main(int, char*[])
{
	int exitStatus = EXIT_SUCCESS;

	WindowData windowData;
	if (setup(WindowWidth, WindowHeight, windowData))
	{
		SystemEventData systemEventData;

		std::vector<Uint8> frameBuffer(FrameBufferPitch * NumDotsHeight);
		std::vector<Color> colorBuffer(NumDotsWidth * NumDotsHeight);

		Maze maze(NumDotsWidth, NumDotsHeight);

		Uint32 prevFrameTime = SDL_GetTicks();

		while (!systemEventData.quit)
		{
			processEvents(systemEventData);

			const Uint32 frameStartTime = SDL_GetTicks();
			const Uint32 deltaTime = frameStartTime - prevFrameTime;

			if (deltaTime > MillisecondsPerRender)
			{
				maze.update(deltaTime, systemEventData.keyboard);
				maze.draw(colorBuffer);

				render(windowData, colorBuffer, frameBuffer);

				prevFrameTime = frameStartTime;
			}
		}
	}
	else
	{
		exitStatus = EXIT_FAILURE;
	}

	shutdown(windowData);

	return exitStatus;
}

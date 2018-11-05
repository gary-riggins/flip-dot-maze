#include "Maze.hpp"

#include <cmath>
#include <iostream>
#include <utility>

const char *const WindowTitle = "Flip Dots Maze";

static const double TilesPerMillisecondSpeed = 3.0 / 1000.0;
static const double RadiansPerMillisecondRotation = 1.0 / 1000.0;

static const int MazeSideTiles = 22;

static int mazeTiles[MazeSideTiles][MazeSideTiles] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
	{1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1},
	{1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1},
	{1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
	{1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
	{1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1},
	{1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1},
	{1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1},
	{1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1},
	{1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1},
	{1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1},
	{1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1},
	{1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1},
	{1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1},
	{1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
	{1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

const Color Maze::background = Color::Black;
const Color Maze::northSouthColor = Color::White;
const Color Maze::eastWestColor = Color::Alternating;

Maze::Maze(int screenWidth, int screenHeight) :
	screenWidth(screenWidth),
	screenHeight(screenHeight),
	positionX(MazeSideTiles - 3), positionY(MazeSideTiles - 3),
	directionX(-1), directionY(0),
	screenPlaneX(0), screenPlaneY(0.66)
{}

void Maze::update(uint32_t millisecElapsed, const Keyboard& keyboard)
{
	const double movementSpeed = millisecElapsed* TilesPerMillisecondSpeed;
	const double rotationSpeed = millisecElapsed * RadiansPerMillisecondRotation;

	if (keyboard.isPressed(Key::UpArrow))
	{
		if (mazeTiles[static_cast<int>(positionX + directionX * movementSpeed)][static_cast<int>(positionY)] == 0)
		{
			positionX += directionX * movementSpeed;
		}

		if (mazeTiles[static_cast<int>(positionX)][static_cast<int>(positionY + directionY * movementSpeed)] == 0)
		{
			positionY += directionY * movementSpeed;
		}
	}

	if (keyboard.isPressed(Key::DownArrow))
	{
		if (mazeTiles[int(positionX - directionX * movementSpeed)][int(positionY)] == 0)
		{
			positionX -= directionX * movementSpeed;
		}

		if (mazeTiles[int(positionX)][int(positionY - directionY * movementSpeed)] == 0)
		{
			positionY -= directionY * movementSpeed;
		}
	}

	if (keyboard.isPressed(Key::RightArrow))
	{
		const double sine = sin(-rotationSpeed);
		const double cosine = cos(-rotationSpeed);

		const double prevDirectionX = directionX;
		directionX = directionX * cosine - directionY * sine;
		directionY = prevDirectionX * sine + directionY * cosine;

		const double prevScreenPlaneX = screenPlaneX;
		screenPlaneX = screenPlaneX * cosine - screenPlaneY * sine;
		screenPlaneY = prevScreenPlaneX * sine + screenPlaneY * cosine;
	}

	if (keyboard.isPressed(Key::LeftArrow))
	{
		const double sine = sin(rotationSpeed);
		const double cosine = cos(rotationSpeed);

		double prevDirectionX = directionX;
		directionX = directionX * cosine - directionY * sine;
		directionY = prevDirectionX * sine + directionY * cosine;

		double prevScreenPlaneX = screenPlaneX;
		screenPlaneX = screenPlaneX * cosine - screenPlaneY * sine;
		screenPlaneY = prevScreenPlaneX * sine + screenPlaneY * cosine;
	}
}

void Maze::draw(std::vector<Color>& colorBuffer) const
{
	std::fill(colorBuffer.begin(), colorBuffer.end(), background);

	for (int x = 0; x < screenWidth; ++x)
	{
		const double cameraX = 2 * x / static_cast<double>(screenWidth) - 1;
		const double raycastDirectionX = directionX + screenPlaneX * cameraX;
		const double raycastDirectionY = directionY + screenPlaneY * cameraX;

		int tileX = static_cast<int>(positionX);
		int tileY = static_cast<int>(positionY);

		double distanceNearestX;
		double distanceNearestY;

		const double rayDistancePerX = std::abs(1 / raycastDirectionX);
		const double rayDistancePerY = std::abs(1 / raycastDirectionY);

		// +/ 1
		int stepDirectionX;
		int stepDirectionY;

		bool raycastFoundTile = false;
		bool hitNorthSouthWall;

		if (raycastDirectionX < 0)
		{
			stepDirectionX = -1;
			distanceNearestX = (positionX - tileX) * rayDistancePerX;
		}
		else
		{
			stepDirectionX = 1;
			distanceNearestX = (tileX + 1.0 - positionX) * rayDistancePerX;
		}

		if (raycastDirectionY < 0)
		{
			stepDirectionY = -1;
			distanceNearestY = (positionY - tileY) * rayDistancePerY;
		}
		else
		{
			stepDirectionY = 1;
			distanceNearestY = (tileY + 1.0 - positionY) * rayDistancePerY;
		}

		do
		{
			if (distanceNearestX < distanceNearestY)
			{
				distanceNearestX += rayDistancePerX;
				tileX += stepDirectionX;
				hitNorthSouthWall = false;
			}
			else
			{
				distanceNearestY += rayDistancePerY;
				tileY += stepDirectionY;
				hitNorthSouthWall = true;
			}

			if (mazeTiles[tileX][tileY] != 0)
			{
				raycastFoundTile = true;
			}
		} while (!raycastFoundTile);

		// To avoid fisheye, calculate distance projected on camera direction
		double perpWallDist;
		if (hitNorthSouthWall == 0)
		{
			perpWallDist = (tileX - positionX + (1 - stepDirectionX) / 2) / raycastDirectionX;
		}
		else
		{
			perpWallDist = (tileY - positionY + (1 - stepDirectionY) / 2) / raycastDirectionY;
		}

		int lineHeight = (int)(screenHeight / perpWallDist);

		int lineBottom = -lineHeight / 2 + screenHeight / 2;
		if (lineBottom < 0)
		{
			lineBottom = 0;
		}
		int lineTop = lineHeight / 2 + screenHeight / 2;
		if (lineTop >= screenHeight)
		{
			lineTop = screenHeight - 1;
		}

		Color color = (hitNorthSouthWall == 0) ? northSouthColor : eastWestColor;
		drawVerticalLine(colorBuffer, x, lineBottom, lineTop, color);
	}
}

void Maze::drawVerticalLine(std::vector<Color>& colorBuffer, int x, int y1, int y2, Color color) const
{
	Color *buffer = colorBuffer.data() + y1 * screenWidth + x;

	for (int y = y1; y <= y2; ++y)
	{
		*buffer = color;
		buffer += screenWidth;
	}
}
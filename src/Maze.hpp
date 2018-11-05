#ifndef MAZE_HPP
#define MAZE_HPP
#pragma once

#include <cstddef>
#include <vector>

#include "Keyboard.hpp"
#include "main.hpp"


class Maze
{
public:
	Maze(int screenWidth, int screenHeight);

	void update(uint32_t millisecElapsed, const Keyboard& keyboard);
	void draw(std::vector<Color>& colorBuffer) const;

private:
	void drawVerticalLine(std::vector<Color>& colorBuffer, int x, int y1, int y2, Color color) const;

	const int screenWidth;
	const int screenHeight;

	double positionX;
	double positionY;

	double directionX;
	double directionY;

	double screenPlaneX;
	double screenPlaneY;

	const static Color background;
	const static Color northSouthColor;
	const static Color eastWestColor;
};

#endif

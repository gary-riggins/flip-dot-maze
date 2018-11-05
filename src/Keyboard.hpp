#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
#pragma once

#include <bitset>

enum class Key : size_t
{
	UpArrow,
	DownArrow,
	LeftArrow,
	RightArrow
};

class Keyboard
{
public:
	Keyboard();

	void press(Key key);
	void release(Key key);

	bool isPressed(Key key) const;
	bool wasJustPressed(Key key) const;

private:
	// Note: size must number of members in Key
	std::bitset<4> pressed;
	std::bitset<4> justPressed;
};

#endif

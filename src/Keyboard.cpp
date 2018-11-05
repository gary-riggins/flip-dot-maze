#include "Keyboard.hpp"

Keyboard::Keyboard() :
	pressed{ 0 },
	justPressed{ 0 }
{}

void Keyboard::press(Key key)
{
	size_t index = static_cast<size_t>(key);

	justPressed[index] = !pressed[index];
	pressed[index] = true;
}

void Keyboard::release(Key key)
{
	size_t index = static_cast<size_t>(key);

	justPressed[index] = false;
	pressed[index] = false;
}

bool Keyboard::isPressed(Key key) const
{
	return pressed[static_cast<size_t>(key)];
}

bool Keyboard::wasJustPressed(Key key) const
{
	return justPressed[static_cast<size_t>(key)];
}

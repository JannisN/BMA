#include "Keyboard.h"

namespace Crystal
{
	bool Keyboard::keysDown[348];
	std::list<KeyboardKeys> Keyboard::keysPressed;

	bool Keyboard::isKeyDown(KeyboardKeys key)
	{
		return keysDown[key];
	}

	bool Keyboard::isKeyPressed(KeyboardKeys key)
	{
		return std::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end();
	}

	void Keyboard::setKeyState(KeyboardKeys key, bool option)
	{
		keysDown[key] = option;
	}

	void Keyboard::setKeyPressed(KeyboardKeys key)
	{
		keysPressed.push_back(key);
	}

	void Keyboard::update()
	{
		keysPressed.clear();
	}
}
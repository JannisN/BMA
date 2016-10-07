#include "Mouse.h"

namespace Crystal
{
	class Application;

	Vec2D Mouse::pos;
	Vec2D Mouse::dPos;
	Vec2D Mouse::oldPos;

	bool Mouse::wrap;

	bool Mouse::buttonsDown[3];
	std::list<MouseButtons> Mouse::buttonsPressed;

	bool Mouse::isButtonDown(MouseButtons button)
	{
		return buttonsDown[button];
	}

	bool Mouse::isButtonPressed(MouseButtons button)
	{
		return std::find(buttonsPressed.begin(), buttonsPressed.end(), button) != buttonsPressed.end();
	}

	void Mouse::setButtonState(MouseButtons button, bool option)
	{
		buttonsDown[button] = option;
	}

	void Mouse::setButtonPressed(MouseButtons button)
	{
		buttonsPressed.push_back(button);
	}

	void Mouse::update(int width, int height, Timestamp timestamp)
	{
		if (wrap)
		{
			dPos = Vec2D((pos - oldPos).x, (pos - oldPos).y);
			//dPos = pos;
			//dPos = (Vec2D(width / 2, height / 2) - pos);
			oldPos = pos;
			//Reporter::report(floatToString(pos.x) + " " + floatToString(pos.y));
		}
		else
		{
			dPos = Vec2D((oldPos - pos).x, (oldPos - pos).y);
			oldPos = pos;
		}
		buttonsPressed.clear();
	}

	void Mouse::setPos(Vec2D pos)
	{
		Mouse::pos = pos;
	}

	void Mouse::addPos(Vec2D pos, Vec2D size)
	{
		Mouse::pos = Mouse::pos + Vec2D(size.x / 2, size.y / 2) - pos;
	}

	Vec2D Mouse::getPos()
	{
		return pos;
	}

	Vec2D Mouse::getDeltaPos()
	{
		return dPos;
	}

	void Mouse::setWrap(bool option)
	{
		wrap = option;
	}

	bool Mouse::isWrapped()
	{
		return wrap;
	}
}
#pragma once

#include "Vec2D.h"
#include "Timestamp.h"
#include "Reporter.h"

#include <list>

namespace Crystal
{
	enum MouseButtons
	{
		ButtonLeft = 0,
		ButtonRight = 1,
		ButtonMiddle = 2
	};

	static class Mouse
	{
	public:
		static bool isButtonDown(MouseButtons button);
		static bool isButtonPressed(MouseButtons button);
		static void setButtonState(MouseButtons button, bool option);
		static void setButtonPressed(MouseButtons button);
		static void update(int width, int height, Timestamp timestamp);
		static void setPos(Vec2D pos);
		static void addPos(Vec2D pos, Vec2D size);
		static Vec2D getPos();
		static Vec2D getDeltaPos();
		static void setWrap(bool option);
		static bool isWrapped();

	private:
		static Vec2D pos;
		static Vec2D dPos;
		static Vec2D oldPos;

		static bool paused;
		static bool wrap;

		static bool buttonsDown[3];
		static std::list<MouseButtons> buttonsPressed;
	};
}
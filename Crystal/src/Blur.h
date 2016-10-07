#pragma once

#include "ScreenEffect.h"

namespace Crystal
{
	class Blur : public ScreenEffect
	{
	public:
		Blur();
		void render(Renderer* renderer, Texture texture, Texture texture2);
	private:
		static char* blurSource;
	};
}
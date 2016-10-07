#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <time.h>

namespace Crystal
{
	class Timestamp
	{
	public:
		Timestamp();
		void updateTime(); //vlt only namespace?
		float time();
		float getFPS();
		static char* getDateTime();

	private:
		std::chrono::time_point<std::chrono::steady_clock> newTime;
		std::chrono::time_point<std::chrono::steady_clock> oldTime;
		float tickTime;
	};
}
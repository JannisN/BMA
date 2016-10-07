#include "Timestamp.h"

namespace Crystal
{
	Timestamp::Timestamp()
	{
		oldTime = std::chrono::high_resolution_clock::now();
	}

	void Timestamp::updateTime()
	{
		newTime = std::chrono::high_resolution_clock::now();
		tickTime = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - oldTime).count() * 1E-9;
		oldTime = newTime;
	}

	float Timestamp::time()
	{
		return tickTime;
	}

	float Timestamp::getFPS()
	{
		return 1.0 / tickTime;
	}

	char* Timestamp::getDateTime()
	{
		return "";//TODO
	}
}
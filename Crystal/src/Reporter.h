#pragma once

#include "Console.h"
#include "Logger.h"
#include "Utils.h"

namespace Crystal
{
	static class Reporter
	{
	public:
		static void report(std::string text);
		static void report(float value);
		static void errorReport(std::string text);
	};
}
#pragma once

#include "Timestamp.h"

#include <fstream>
#include <string>

namespace Crystal
{
	static class Logger
	{
	public:
		static void write(std::string text);
		static void writeln(std::string text);
		static void newline();
		static const char* endl;

	private:
		static std::ofstream* out;
		static void createLog();
	};
}
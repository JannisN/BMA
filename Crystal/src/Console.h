#pragma once

#include <string>
#include <iostream>

namespace Crystal
{
	static class Console
	{
	public:
		static void write(std::string text);
		static void writeln(std::string text);
		static void newline();
		static const char* endl;
	};
}
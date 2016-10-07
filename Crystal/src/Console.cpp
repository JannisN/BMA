#include "Console.h"

namespace Crystal
{
	void Console::write(std::string text)
	{
		std::cout << text;
	}

	void Console::writeln(std::string text)
	{
		std::cout << text << std::endl;
	}

	void Console::newline()
	{
		std::cout << std::endl;
	}

	const char* Console::endl = "\r\n";
}
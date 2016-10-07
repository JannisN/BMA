#include "Reporter.h"

namespace Crystal
{
	void Reporter::report(std::string text)
	{
//#ifdef DEBUG
		Logger::writeln(text);
		Console::writeln(text);
//#endif
	}

	void Reporter::report(float value)
	{
//#ifdef DEBUG
		report(floatToString(value));
//#endif
	}

	void Reporter::errorReport(std::string text)
	{
//#ifdef DEBUG
		Logger::write("Error: ");
		Logger::writeln(text);
		Console::write("Error: ");
		Console::writeln(text);
//#endif
	}
}
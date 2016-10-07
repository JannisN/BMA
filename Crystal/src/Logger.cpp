#include "Logger.h"

namespace Crystal
{
	std::ofstream* Logger::out;

	void Logger::createLog()
	{
		if (!out)
		{
			//TODO name des files soll timestamp beinhalten
			out = new std::ofstream("../test.txt");
		}
	}

	void Logger::write(std::string text)
	{
		createLog();

		*out << Timestamp::getDateTime() << ": " << text;
	}

	void Logger::writeln(std::string text)
	{
		createLog();

		*out << Timestamp::getDateTime() << ": " << text << endl;
	}

	void Logger::newline()
	{
		createLog();

		*out << endl;
	}

	const char* Logger::endl = "\r\n";
}
#include "Utils.h"

namespace Crystal
{
	std::string floatToString(float value)
	{
		std::ostringstream ss;
		ss << value;
		std::string s(ss.str());
		
		return s;
	}

	float stringToFloat(std::string value)
	{
		return ::atof(value.c_str());
	}

	std::string loadFile(std::string fileName) {
		std::ifstream file;
		file.open((fileName).c_str());

		std::string output;
		std::string line;

		if (file.is_open()) {
			while (file.good()) {
				getline(file, line);
				output.append(line + "\n");
			}
		}
		else
			Reporter::errorReport("Can't open file " + fileName);

		file.close();
		return output;
	}

	float radToDeg(float rad)
	{
		return rad / pi * 180;
	}

	float degToRad(float deg)
	{
		return deg / 180 * pi;
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	std::vector<std::string> split(const std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> strings;

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(str.substr(prev, pos - prev));
			prev = pos + 1;
		}

		// To get the last substring (or only, if delimiter is not found)
		strings.push_back(str.substr(prev));

		return strings;
	}
}
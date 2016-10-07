#pragma once

#include "Reporter.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>

namespace Crystal
{
	std::string floatToString(float value);
	float stringToFloat(std::string value);
	std::string loadFile(std::string fileName);
	float radToDeg(float rad);
	float degToRad(float deg);
	const float pi = 3.14159265;
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string> split(const std::string& str, const std::string& delimiter);
}

//char to float
//float to char
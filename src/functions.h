#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>
#include <sstream>
#include <regex>

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		// remove leading and trailing whitespace
		item = std::regex_replace(item, std::regex("^ +| +$|( ) +"), "$1");
		result.push_back(item);
	}

	return result;
}

std::string ToLower(std::string str)
{
	// make lowercase only
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return str;
}

std::string ToUpper(std::string str)
{
	// make uppercase only
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::toupper(c); });

	return str;
}

// removes all whitespace
std::string RemoveSpaces(std::string str)
{
	// remove all spaces
	str.erase(remove(str.begin(), str.end(), ' '), str.end());

	return str;
}

#endif
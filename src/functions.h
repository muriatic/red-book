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

#endif
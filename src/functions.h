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

// removes leading a trailing whitespace
std::string Strip(std::string str)
{
	// remove leading and trailing whitespace
	return std::regex_replace(str, std::regex("^ +| +$|( ) +"), "$1");
}

// capitalizes the first letter
std::string Capitalize(std::string str)
{
	str[0] = toupper(str[0]);
	return str;
}

std::string CapitalizeTitle(std::string str)
{
	std::vector<std::string> nonCapitalizedWords {"to", "a", "at", "the", "on", "of", "in", "and", "for", "or", "an", "as"};

	std::vector<std::string> words = split(str, ' ');

	std::string title;

	for (int i =0; i < words.size(); i++)
	{
		// first word is always capitalized
		if (i == 0)
		{
			title += Capitalize(words[i]);
			continue;
		}

		// its a prep, etc...
		int IDX = IndexOf(nonCapitalizedWords, ToLower(words[i]));
		if (IDX != -1)
		{
			title += " " + nonCapitalizedWords[IDX];
			continue;
		}

		title += " " + Capitalize(words[i]);
	}

	return title;
}

#endif
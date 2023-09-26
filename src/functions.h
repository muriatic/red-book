#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <filesystem>
#include <numeric>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <ostream>
#include <map>

namespace fs = std::filesystem;

// get index of substring in string
int IndexOf(std::string str, std::string substr)
{
	size_t idx = str.find(substr);

	// return idx if found, else -1
	return idx != std::string::npos ? idx : -1;
}

template <typename T, typename O> std::vector<T> GetKeys(std::map<T, O> mmap)
{
	std::vector<T> keys;
	for (auto const& imap : mmap)
	{
		keys.push_back(imap.first);
	}

	return keys;
}

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

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
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

std::vector<std::string> ListDir(std::string directory)
{
	std::vector<std::string> files;

	for (const auto& entry : fs::directory_iterator(directory))
	{
		files.push_back(entry.path().generic_string());
	}

	return files;
}

std::string ReadLine()
{
	std::string userMessage;
	//std::cin.ignore('\n');
	std::getline(std::cin >> std::ws, userMessage);
	//std::cin.clear();
	return userMessage;
}


template <typename T>
std::string Join(const T& v, const std::string& delim) {
	std::ostringstream s;
	for (const auto& i : v) {
		if (&i != &v[0]) {
			s << delim;
		}
		s << i;
	}
	return s.str();
}

std::string EXEC(const char* cmd)
{
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

std::string EXEC(std::string cmd_str)
{
	const char* cmd = cmd_str.c_str();
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

#endif
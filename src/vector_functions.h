#ifndef VECTORFUNCTIONS_H
#define VECTORFUNCTIONS_H

#include <vector>
#include <string>
#include <iostream>

template <typename T> int IndexOf(std::vector<T> vect, T value)
{
	// check if the value is in the vector at all
	if (std::find(vect.begin(), vect.end(), value) == vect.end())
	{
		return -1;
	}

	auto it = std::find(vect.begin(), vect.end(), value);

	int idx = it - vect.begin();

	return idx;
}

std::vector<std::string> ToLower(std::vector<std::string> vect)
{
	for (std::string str : vect)
	{
		// make lowercase only
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return std::tolower(c); });
	}

	return vect;
}

template <typename T> void PrintVector(std::vector<T> vect)
{
	for (int i = 0; i < vect.size(); i++)
	{
		std::cout << vect[i] << std::endl;
	}
}

#endif
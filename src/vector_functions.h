#ifndef VECTORFUNCTIONS_H
#define VECTORFUNCTIONS_H

#include <vector>
#include <string>

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

void ToLower(std::vector<std::string>& vect)
{
	for (std::string str : vect)
	{
		// make lowercase only
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return std::tolower(c); });
	}
}

#endif
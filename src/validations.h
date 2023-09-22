#pragma once
#include <string>
#include <regex>
#include <format>

#include "person.h"
#include "read_csv.h"
#include "vector_functions.h"

float CompareString(std::string str1, std::string str2)
{
	size_t str1_length = str1.size();
	size_t str2_length = str2.size();

	size_t smaller_length = (str1_length < str2_length) ? str1_length : str2_length;
	size_t greater_length = (str1_length > str2_length) ? str1_length : str2_length;

	int sameCharacters = 0;

	for (int i = 0; i < smaller_length; i++)
	{
		// comparing both characters lowercased
		if (std::tolower(str1[i]) == std::tolower(str2[i]))
		{
			sameCharacters++;
		}
	}
	float similarity = (float)sameCharacters / (float)greater_length;
	return similarity;
}

// takes a vector of strings, a search value, and a similarity percentage 
std::string BestMatch(std::vector<std::string> vect, std::string searchValue, float similarity = 0.8)
{
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect[i] == searchValue)
		{
			return vect[i];
		}
		// i might want to change this in the future but for now we just return if the similarity is over 0.8
		// will reiterate over the successPercents vector and find the highest value
		if (CompareString(vect[i], searchValue) >= similarity)
		{
			return vect[i];
		}
	}

	std::cerr << "No match found in the vector for " << searchValue << std::endl;
	exit(EXIT_FAILURE);
}

std::string ToLower(std::string str)
{
	// make lowercase only
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return str;
}

void ValidatePhoneNumber(std::string& phoneNumber)
{
	std::regex rgx("([0-9]{3})[^0-9]*([0-9]{3})[^0-9]*([0-9]{4})");
	std::smatch m;

	std::regex_search(phoneNumber, m, rgx);

	if (m.size() != 4)
	{
		std::cerr << "ERROR: The areaCode, telephonePrefix, and lineNumber were not found for the phone number provided: " << phoneNumber << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string areaCode = m[1].str();
	std::string telephonePrefix = m[2].str();
	std::string lineNumber = m[3].str();

	phoneNumber = std::format("({}) {} - {}", areaCode, telephonePrefix, lineNumber);
}

void ValidateEmail(std::string &email)
{
	// remove leading and trailing whitespace
	email = std::regex_replace(email, std::regex("^ +| +$|( ) +"), "$1");

	// make lowercase only
	std::transform(email.begin(), email.end(), email.begin(),
		[](unsigned char c) { return std::tolower(c); });
	
	std::regex rgx("([A-Za-z0-9\+\-\_\.]+)\@([A-Za-z0-9\-\.]+)\.([A-Za-z0-9]+)");
	if (!std::regex_match(email, rgx))
	{
		std::cerr << "Email " << email << " is not a valid email, ensure it follows Username@EmailServer.TLD like example@gmail.com.If this email is valid, please change the regular expression" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void ValidateLinkedIns(std::string& linkedin)
{
	// remove all spaces
	std::remove_if(linkedin.begin(), linkedin.end(), isspace);

	// make lowercase only
	std::transform(linkedin.begin(), linkedin.end(), linkedin.begin(),
		[](unsigned char c) { return std::tolower(c); });

	// we're going to just use the normal format and if its empty raise an error
	std::regex rgx("linkedin.com/in/([a-z0-9\-]+)/?");

	std::smatch m;

	std::regex_search(linkedin, m, rgx);

	if (m[1].str() == "")
	{
		std::cerr << "No handle found for " << linkedin << std::endl;
		exit(EXIT_FAILURE);
	}


	linkedin = std::format("https://www.linkedin.com/in/{}/", m[1].str());
}

void ValidateCurrentPositions(std::vector<std::string>& currentPositions)
{
	CSV csv("resources/F23_positions.csv");

	std::vector<std::string> acceptedPositions = csv.getColumn("Positions");
	std::vector<std::string> positionAliases = csv.getColumn("aka");

	if (currentPositions.size() == 0)
	{
		return;
	}

	for (int i = 0; i < currentPositions.size(); i++)
	{
		// remove leading and trailing whitespace
		currentPositions[i] = std::regex_replace(currentPositions[i], std::regex("^ +| +$|( ) +"), "$1");

		if (currentPositions[i] == "")
		{
			return;
		}

		// check if current position is in aliases (we're sending it in as lowercased)
		if (int IDX = IndexOf(ToLower(positionAliases), ToLower(currentPositions[i])) == -1)
		{
			currentPositions[i] = acceptedPositions[IDX];
			continue;
		}

		// at this point the currentPosition is not in the aliases so we will search for the most similar
		// BestMatch() will try to find a match within 0.8 similarity, if not it will just throw an error
		currentPositions[i] = BestMatch(acceptedPositions, currentPositions[i], 0.8);
	}
}

void ValidatePerson(Person &person)
{
	ValidatePhoneNumber(person.phoneNumber);
	ValidateEmail(person.email);
	ValidateLinkedIns(person.linkedin);
	ValidateCurrentPositions(person.currentPositions);
}
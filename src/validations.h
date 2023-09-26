#pragma once
#include <string>
#include <regex>
#include <format>

#include "person.h"
#include "read_csv.h"
#include "vector_functions.h"
#include "functions.h"

float CompareString(std::string str1, std::string str2)
{
	size_t str1_length = str1.size();
	size_t str2_length = str2.size();

	size_t lesser_length = (str1_length < str2_length) ? str1_length : str2_length;

	int sameCharacters = 0;

	for (int i = 0; i < lesser_length; i++)
	{
		// comparing both characters lowercased
		if (std::tolower(str1[i]) == std::tolower(str2[i]))
		{
			sameCharacters++;
		}
	}
	float similarity = (float)sameCharacters / (float)lesser_length;
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

struct Validations{
	void ValidateName(std::string& name)
	{
		name = Strip(name);
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
		email = Strip(email);

		// make lowercase only
		email = ToLower(email);
		
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
		linkedin = RemoveSpaces(linkedin);

		// make lowercase only
		linkedin = ToLower(linkedin);

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
			currentPositions[i] = Strip(currentPositions[i]);

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

	void ValidateMajors(std::vector<std::string>& majors) 
	{
		CSV csv("resources/majors.csv");

		std::vector<std::string> acceptedMajors = csv.getColumn("Majors");

		if (majors.size() == 0)
		{
			return;
		}

		for (int i = 0; i < majors.size(); i++)
		{
			// remove leading and trailing whitespace
			majors[i] = Strip(majors[i]);

			if (majors[i] == "")
			{
				return;
			}

			// at this point the majors is not in the aliases so we will search for the most similar
			// BestMatch() will try to find a match within 0.8 similarity, if not it will just throw an error
			majors[i] = BestMatch(acceptedMajors, majors[i], 0.8);
		}

	}

	void ValidateMinors(std::vector<std::string>& minors)
	{
		CSV csv("resources/minors.csv");

		std::vector<std::string> acceptedMinors = csv.getColumn("Minors");

		if (minors.size() == 0)
		{
			return;
		}

		for (int i = 0; i < minors.size(); i++)
		{
			// remove leading and trailing whitespace
			minors[i] = Strip(minors[i]);

			if (minors[i] == "")
			{
				return;
			}

			// at this point the majors is not in the aliases so we will search for the most similar
			// BestMatch() will try to find a match within 0.8 similarity, if not it will just throw an error
			minors[i] = BestMatch(acceptedMinors, minors[i], 0.8);
		}

	}

	void ValidateGradDate(std::string& gradDate)
	{
		// gradDate has been pre-validated so its just mmm-yy
		std::vector<std::string> monthYear = split(gradDate, '-');
		std::string month = monthYear[0];

		// add 20 in front since 2 digit year
		std::string year = "20" + monthYear[1];

		std::vector<std::string> months_mmm{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		std::vector<std::string> months_MMM{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

		int mmmIDX = IndexOf(months_mmm, month);

		gradDate = std::format("{} {}", months_MMM[mmmIDX], year);
	}

	// this just makes the pronouns all caps and removes all spaces
	void ValidatePronouns(std::string& pronouns)
	{
		pronouns = ToUpper(pronouns);
		pronouns = RemoveSpaces(pronouns);
	}

	// really just removes the whitespaces and that's it
	void ValidateProfessionalECs(std::vector<std::string>& proECs)
	{
		for (int i = 0; i < proECs.size(); i++)
		{
			proECs[i] = Strip(proECs[i]);
		}
	}

	// really just removes the whitespaces and that's it
	void ValidateAskMeAbout(std::vector<std::string>& askMeAbout)
	{
		for (int i = 0; i < askMeAbout.size(); i++)
		{
			askMeAbout[i] = Strip(askMeAbout[i]);
			askMeAbout[i] = CapitalizeTitle(askMeAbout[i]);
		}
	}

	// remove whitespace, capitalize
	void ValidateGradProgram(std::string& str)
	{
		str = Strip(str);
		str = CapitalizeTitle(str);
	}

	void ValidateResumeFile(std::string name, std::string& resumeFile)
	{
		resumeFile = std::format("resumes/resume - {}.pdf", name);

		int IDX = IndexOf(resumeFiles, resumeFile);
		if (IDX == -1)
		{
			std::cerr << "Resume for " << name << " was not found. Was expecting " << resumeFile << std::endl;
			exit(EXIT_FAILURE);
		}

		// last second remove the directory name
		resumeFile = std::format("resume - {}.pdf", name);
	}

	void ValidateImageFile(std::string name, std::string& imageFile)
	{
		// kinda weird way of doing this but we're gonna go up to but not including file extension but keep the .
		imageFile = std::format("professional_headshots/image - {}.", name);

		// we're gonna get the best match with a requirement of 100% accuracy because it'll stop at the shorter length
		imageFile = BestMatch(imageFiles, imageFile, 1.0f);
	}

	Validations(Person &person)
	{
		ValidateName(person.name);
		ValidatePhoneNumber(person.phoneNumber);
		ValidateEmail(person.email);
		ValidateLinkedIns(person.linkedin);
		ValidateCurrentPositions(person.currentPositions);
		ValidateMajors(person.majors);
		ValidateMinors(person.minors);
		ValidateGradDate(person.gradDate);
		ValidatePronouns(person.pronouns);
		ValidateProfessionalECs(person.professionalInvolvement);
		ValidateAskMeAbout(person.askMeAbout);
		ValidateGradProgram(person.gradProgram);
		ValidateResumeFile(person.name, person.resumeFile);
		ValidateImageFile(person.name, person.imageFile);
	}

	static std::vector<std::string> resumeFiles;
	static std::vector<std::string> imageFiles;
};
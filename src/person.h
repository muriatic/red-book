#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <fstream>

#include "functions.h"


class Person
{
	void parseRow(Row row)
	{
		name = row.getValue("Full Name");
		majors = split(row.getValue("Major(s)"), '/');
		minors = split(row.getValue("Minor(s)"), '/');
		gradDate = row.getValue("Expected Graduation Year");
		pronouns = row.getValue("Pronouns");
		phoneNumber = row.getValue("Phone Number");
		email = row.getValue("Email");
		linkedin = row.getValue("LinkedIn");
		gradProgram = row.getValue("Graduate Program");
		professionalInvolvement = split(row.getValue("Other Professional Extracurriculars Involvement"), '/');
		askMeAbout = split(row.getValue("Ask me about... (Professional Information)"), '/');
		currentPositions = split(row.getValue("Current AKPsi Position(s)"), '/');
	}

	std::string CreateRow(std::string line)
	{
		std::string row;
		row += line;
		return row;
	}
	std::string CreateRow(std::vector<std::string> line)
	{
		std::string row;
		row += "<";
		row += Join(line, ",");
		row += ">";
		return row;
	}
	std::string CreateRow(std::vector<std::vector<std::string>> line)
	{
		std::string row;
		row += "<";
		for (std::vector<std::string> entry : line)
		{
			row += "<";
			row += Join(entry, ",");
			row += ">";
		}
		row += ">";
		return row;
	}

public:
	Person(Row row)
	{
		parseRow(row);
	}

	void Save(std::string saveName = "")
	{
		if (saveName == "")
		{
			saveName = "brothers/" + name + ".brother";
		}

		std::vector<std::string> rows;
		rows.push_back(CreateRow(name));
		rows.push_back(CreateRow(majors));
		rows.push_back(CreateRow(minors));
		rows.push_back(CreateRow(gradDate));
		rows.push_back(CreateRow(pronouns));
		rows.push_back(CreateRow(phoneNumber));
		rows.push_back(CreateRow(email));
		rows.push_back(CreateRow(linkedin));
		rows.push_back(CreateRow(professionalInvolvement));
		rows.push_back(CreateRow(askMeAbout));
		rows.push_back(CreateRow(currentPositions));
		rows.push_back(CreateRow(gradProgram));
		rows.push_back(CreateRow(resumeFile));
		rows.push_back(CreateRow(imageFile));
		rows.push_back(CreateRow(resumeInfo));
		
		std::string contents = Join(rows, "\n");

		std::fstream file(saveName, std::ios::out);
		file << contents;
	}

	std::string name;
	std::vector<std::string> majors;
	std::vector<std::string> minors;
	std::string gradDate;
	std::string pronouns;
	std::string phoneNumber;
	std::string email;
	std::string linkedin;
	std::vector<std::string> professionalInvolvement;
	std::vector<std::string> askMeAbout;
	std::vector<std::string> currentPositions;
	std::string gradProgram;
	std::string resumeFile;
	std::string imageFile;
	std::vector<std::vector<std::string>> resumeInfo;
};

#endif
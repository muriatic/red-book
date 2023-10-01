#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <fstream>
#include <map>

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

	void parseVector(std::string line, std::vector<std::string> &vect)
	{
		//
		if (!has_prefix(line, "<") && !has_suffix(line, ">")) {
			vect = split(line, ',');
		}
		else {
			std::string tempLine;
			// removes < > at beginning and end
			tempLine = line.substr(1, line.size() - 2);
			vect = split(tempLine, ',');
		}
	}
	void parseMap(std::string line, std::vector<std::map<std::string, std::string>> &mapVector)
	{
		// remove { & } at beginning and end 
		line = line.substr(1, line.size() - 2);

		std::vector<std::string> entries = split(line, "}{");

		for (int i = 0; i < entries.size(); i++)
		{
			std::vector<std::string> keySets = split(entries[i], ';');

			std::map<std::string, std::string> tempMap;

			for (std::string keySet : keySets)
			{
				//std::vector<std::string> keyANDset = split(keySet, ':');

				int firstColonIDX = IndexOf(keySet, ":");
				std::string key = keySet.substr(0, firstColonIDX);
				key = Strip(key, '"');

				std::string value = keySet.substr(firstColonIDX+1, keySet.size() - firstColonIDX);
				value = Strip(value, '"');


				tempMap.insert({ key, value });
			}

			mapVector.push_back(tempMap);
		}
	}
	void parseLines(std::vector<std::string> lines)
	{
		// all files should be 16 lines (if they have no work experience it'll be 15)
		if (lines.size() == 15)
		{
			lines.push_back("");
		}
		if (lines.size() != 16)
		{
			std::cerr << "a non-16 line vector was provided. " << lines.size() << " lines for " << lines[0] << std::endl;
			exit(EXIT_FAILURE);
		}
		name = lines[0];

		parseVector(lines[1], majors);
		parseVector(lines[2], minors);
		gradDate = lines[3];
		pronouns = lines[4];
		phoneNumber = lines[5];
		email = lines[6];
		linkedin = lines[7];
		parseVector(lines[8], professionalInvolvement);
		parseVector(lines[9], askMeAbout);
		parseVector(lines[10], currentPositions);
		gradProgram = lines[11];
		resumeFile = lines[12];
		imageFile = lines[13];
		// i dont think im gonna need the resume info line so im going to skip over it
		if (Strip(lines[15]) != "")
		{
			parseMap(lines[15], parsedResumeInfo);
		}
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
		row += Join(line, ",");
		return row;
	}
	std::string CreateRow(std::vector<std::vector<std::string>> line)
	{
		std::string row;
		for (std::vector<std::string> entry : line)
		{
			row += "<";
			row += Join(entry, ",");
			row += ">";
		}
		return row;
	}
	std::string CreateRow(std::vector<std::map<std::string, std::string>> line)
	{
		std::string row;
		for (std::map<std::string, std::string> entry : line)
		{
			row += "{";
			int count = entry.size();
			for (auto m : entry)
			{
				row += "\"";
				row += m.first;
				row += "\"";
				row += ":";
				row += "\"";
				row += m.second;
				row += "\"";

				// this ensures that commas are only in between entries
				if (count != 1)
				{
					row += ";";
				}

				count--;
			}
			row += "}";
		}

		return row;
	}

public:
	Person(Row row)
	{
		parseRow(row);
	}
	Person(std::vector<std::string> lines)
	{
		parseLines(lines);
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
		rows.push_back(CreateRow(parsedResumeInfo));
		
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
	std::vector<std::map<std::string, std::string>> parsedResumeInfo;
};

#endif
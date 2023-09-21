#include <string>
#include <vector>
#include <sstream>


std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}


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
public:
	Person(Row row)
	{
		parseRow(row);
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
};
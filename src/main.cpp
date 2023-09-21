#include <iostream>
#include <vector>

#include "read_csv.h"
#include "person.h"
#include "validations.h"

//! note all CSVs MUST be utf-8 
//! not utf-8 with BOM, etc
//! CSVs must have one entry per line, sometimes they get split which screws up the system

int main()
{
	std::vector<Person> brothers;

	// at this point you must make sure that the CSV doesn't have any anomalies like using , instead of / as a delimiter
	CSV csv("resources/Red Book Questions.csv");
	
	// create the list of brothers
	// starting with one to skip the title row
	for (int i = 1; i < csv.rowCount; i++)
	{
		Row row = csv.getRow(i);

		// we are only adding rows with a name
		if (row.getValue("Full Name") != "")
		{
			brothers.push_back(Person(row));
		}
	}


	// validate each brother
	for (int i = 0; i < brothers.size(); i++)
	{
		ValidatePerson(brothers[i]);
	}


	return 0;
}
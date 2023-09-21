#include <iostream>
#include <vector>

#include "read_csv.h"
#include "person.h"

int main()
{
	std::vector<Person> brothers;

	CSV csv("resources/Red Book Questions.csv");
	
	// starting with one to skip the title row
	for (int i = 1; i < csv.rowCount; i++)
	{
		Row row = csv.getRow(i);
		brothers.push_back(Person(row));
	}

	return 0;
}
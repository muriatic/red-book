#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

#include "read_csv.h"
#include "person.h"
#include "validations.h"
#include "resume_parser.h"
#include "functions.h"
#include "multithreader.cpp"

int threads = 2;


namespace fs = std::filesystem;

//! note all CSVs MUST be utf-8 
//! not utf-8 with BOM, etc
//! CSVs must have one entry per line, sometimes they get split which screws up the system

std::vector<std::string> Validations::resumeFiles;
std::vector<std::string> Validations::imageFiles;

std::string brotherFolder = "brothers";
std::string rawImageFolder = "professional_headshots";

std::filesystem::path CWD = std::filesystem::current_path();

void CreateBrothers(std::vector<Person>& brothers)
{
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
	std::cout << "Brother Data Loaded";
	std::cin.get();

	// gets resume files
	Validations::resumeFiles = ListDir("resumes");

	// gets image files
	Validations::imageFiles = ListDir("professional_headshots");

	std::cout << "Resume and Professional Headshot Files FOUND" << std::endl;

	// validate each brother
	for (int i = 0; i < brothers.size(); i++)
	{
		Validations validations(brothers[i]);
	}

	std::cout << "Brothers Validated";
	std::cin.get();

	GetResumeInfo(brothers);

	// read resume info for each brother
	for (int i = 0; i < brothers.size(); i++)
	{
		brothers[i].Save();
	}
}

void ReadBrothers(std::vector<Person>& brothers)
{
	std::vector<std::string> brotherFiles;
	{
		std::vector<std::string> files = ListDir(brotherFolder);

		for (int i = 0; i < files.size(); i++)
		{
			if (has_suffix(files[i], ".brother"))
			{
				brotherFiles.push_back(files[i]);
			}
		}
	}

	if (brotherFiles.size() == 0)
	{
		std::cerr << "Failed to read any brothers in " << brotherFolder << std::endl;
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < brotherFiles.size(); i++)
	{
		std::fstream brotherFile(brotherFiles[i]);
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(brotherFile, line))
		{
			lines.push_back(line);
		}

		brothers.push_back({ lines });
	}
}

int main()
{
	std::vector<Person> brothers;

	ReadBrothers(brothers);
	
	std::vector<std::string> brotherNames;
	std::vector<std::string> brotherFileNames;
	// process the photos
	for (int i = 0; i < brothers.size(); i++)
	{
		brotherNames.push_back(brothers[i].name);

		int subdirectoryIDX = IndexOf(brothers[i].imageFile, "/");
		subdirectoryIDX = (subdirectoryIDX == -1) ? IndexOf(brothers[i].imageFile, "\\") : subdirectoryIDX;

		if (subdirectoryIDX != -1)
		{
			brothers[i].imageFile = brothers[i].imageFile.substr(subdirectoryIDX+1, brothers[i].imageFile.size() - subdirectoryIDX);
		}

		fs::path imagePath(brothers[i].imageFile);
		fs::path imageFolderPath("professional_headshots");

		fs::path fullPath = CWD / imageFolderPath / imagePath;

		brotherFileNames.push_back(brothers[i].imageFile);
	}

	ThreadHandler thread_handler(brotherNames, brotherFileNames, "professional_headshots", "processed_images", threads);

	thread_handler.CreateThreads();


	//std::vector<std::string> processedHeadshots = thread_handler.ConvertThreadResultsToVect();

	//PrintVector(processedHeadshots);

	return 0;
}
#pragma once

#include <unordered_map>
#include <iostream>
#include <vector>
#include <regex>

#include "person.h"
#include "functions.h"

struct ExperienceLine
{
    // index
    int IDX;
    std::string left, right;
};

/*std::vector <std::unordered_map<std::string, std::string>>*/void ReadResume(Person &person)
{
    std::vector<std::string> cmds = { "executables\\pdf-reader.exe", "-dir", "\"resumes\"", "-id", ("\"" + person.name + "\""), "-pdfs", ("\"" + person.resumeFile + "\""), "-threads", "\"2\""};
    std::string cmd = Join(cmds, " ");
    //std::cout << cmd << std::endl;
	std::string output = EXEC(cmd);
	//std::cout << output << std::endl;
}

void ReadResumes(std::vector<Person> &brothers, std::string& output)
{
    // prepare the command line aspects
    std::vector<std::string> names;
    std::vector<std::string> fileNames;
    std::string resumeFolder = "resumes";
    std::string threads = "2";
    std::string EXEPath = "executables\\pdf-reader.exe";
    
    for (int i = 0; i < brothers.size(); i++)
    {
        names.push_back(brothers[i].name);
        fileNames.push_back(brothers[i].resumeFile);
    }

    std::string delimitedNames = Join(names, ";");
    std::string delimitedFileNames = Join(fileNames, ";");

    std::vector <std::string> commands = { EXEPath, "-dir", ("\"" + resumeFolder + "\""), "-id", ("\"" + delimitedNames + "\""), "-pdfs", ("\"" + delimitedFileNames + "\""), "-threads", ("\"" + threads + "\"") };
    std::string cmd = Join(commands, " ");
    output = EXEC(cmd);
}

// potential user input template 
//template <typename T> auto GetUserInput(&T input, std::string prompt)
//{
//    std::cout << prompt << std::endl;
//
//    while (true)
//    {
//        std::cin >> input;
//        if (!std::cin.fail())
//        {
//            break;
//        }
//        else
//        {
//            std::cout << "Incorrect input type, please input a value of type: " << std::typeid(T).name() << std::endl;
//            std::cin.clear();
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//        }
//    }
//}

std::string FormatDate(std::string date)
{
    std::string formattedDate;

    std::vector<std::string> dates = split(date, ';');
    if (dates.size() != 2)
    {
        std::cerr << "2 dates were not provided and separated by a semicolon." << std::endl;
        exit(EXIT_FAILURE);
    }

    formattedDate = dates[0] + " - " + dates[1];

    return formattedDate;
}

void ManualResumeInput(Person& person)
{
    int numberOfEntries;
    std::cout << "For " << person.name << ", how many job entries would you like to enter? (3 max)" << std::endl;

    while (true)
    {
        std::cin >> numberOfEntries;
        if (!std::cin.fail() && (numberOfEntries >= 0 && numberOfEntries <= 3))
        {
            break;
        }
        else
        {
            std::cout << "Incorrect number of entries please type 0, 1, 2, or 3" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::vector<std::vector<std::string>> jobEntries;
    for (int i = 0; i < numberOfEntries; i++)
    {
        std::vector<std::string> jobEntry;
        std::cout << "Enter company name: " << std::endl;

        jobEntry.push_back(ReadLine());

        std::cout << "Enter date range separated by semicolon (e.g. 2015 - 2020 would be entered as 2015;2020): " << std::endl;

        std::string formattedDate = FormatDate(ReadLine());

        jobEntry.push_back(formattedDate);

        std::cout << "Enter Job title: " << std::endl;
        jobEntry.push_back(ReadLine());

        std::cout << "Enter location: " << std::endl;
        jobEntry.push_back(ReadLine());

        std::cout << "Confirm that the following information is correct:" << std::endl;
        std::cout << "(0) Nothing is wrong" << std::endl;
        std::cout << "(1) Company Name: " << jobEntry[0] << std::endl;
        std::cout << "(2) Date range: " << jobEntry[1] << std::endl;
        std::cout << "(3) Job title: " << jobEntry[2] << std::endl;
        std::cout << "(4) Location: " << jobEntry[3] << std::endl;
        jobEntries.push_back(jobEntry);
    }

    person.resumeInfo = jobEntries;
}

void ParseResumeEntry(Person& person, std::string& resumeEntry)
{
    //std::vector<std::string> lines = split(resumeEntry, '\n');
    //std::vector<std::string> words = split(ToLower(resumeEntry), ' ');
    std::vector<std::string> workExperienceTitles = { "leadership & involvement", "work experience", "workexperience", "professional experience", "experience", "professionalexperience" };
    std::vector<std::string> sectionTitles = { "additional information", "projects", "additionalinformation", "other experience", "accomplishments" };

    // find the experience titles in the resume entry
    std::vector<unsigned int> experienceTitleIDXs;
    for (std::string title : workExperienceTitles)
    {
        int IDX = IndexOf(ToLower(resumeEntry), title);
        if (IDX == -1)
        {
            continue;
        }

        experienceTitleIDXs.push_back(IDX);
        
    }

    // if we can't find the experience titles in their resumes, just make it manual
    if (experienceTitleIDXs.size() == 0)
    {
        ManualResumeInput(person);
        return;
    }

    // get first occurrence of work experience title
    unsigned int experienceTitleIDX = *min_element(experienceTitleIDXs.begin(), experienceTitleIDXs.end());

    std::string slicedResumeEntry = resumeEntry.substr(experienceTitleIDX, resumeEntry.size() - experienceTitleIDX);


    // find the next section title
    std::vector<unsigned int> nextTitleIDXs;
    for (std::string title : sectionTitles)
    {
        int IDX = IndexOf(ToLower(slicedResumeEntry), title);

        if (IDX == -1)
        {
            continue;
        }

        nextTitleIDXs.push_back(IDX);
    }

    // if we can't find the next section in their resumes, just make it manual
    if (nextTitleIDXs.size() == 0)
    {
        ManualResumeInput(person);
        return;
    }


    // get first occurrence of next section title
    unsigned int nextTitleIDX = *min_element(nextTitleIDXs.begin(), nextTitleIDXs.end());

    std::string experienceSection = resumeEntry.substr(experienceTitleIDX, nextTitleIDX + 1);
    std::vector<std::string> experienceLines = split(experienceSection, '\n');


    // add all lines that match this format
    std::regex rgx("(.*?)[\s]{2,}(.*)");
    std::vector<ExperienceLine> matches;

    for (int i = 0; i < experienceLines.size(); i++)
    {
        std::smatch m;
        std::regex_search(experienceLines[i], m, rgx);
        if (m.size() == 3)
        {
            matches.push_back({ i, m[1].str(), m[2].str() });
        }
    }


    std::vector<ExperienceLine> correctedMatches;
    unsigned int lastIDX = 0;
    int currentGroup = 0;
    // find the lines that are adjacent
    for (ExperienceLine line : matches)
    {
        unsigned int currentIDX = line.IDX;
        correctedMatches.push_back({currentGroup, line.left, line.right});
        
        // are they one apart
        if ((currentIDX - lastIDX) == 1)
        {
            currentGroup++;
        }

        lastIDX = currentIDX;
    }

    // group entries together
    std::map <int, std::vector<std::string>> mmap;
    for (int i = 0; i < correctedMatches.size(); i++)
    {
        if (IndexOf(GetKeys(mmap), correctedMatches[i].IDX) == -1)
        {
            mmap.insert({ correctedMatches[i].IDX, {correctedMatches[i].left, correctedMatches[i].right}});
        }
        else
        {
            std::vector<std::string> tempVector = mmap.at(correctedMatches[i].IDX);
            tempVector.push_back(correctedMatches[i].left);
            tempVector.push_back(correctedMatches[i].right);
            mmap.insert_or_assign(correctedMatches[i].IDX, tempVector);
        }
    }



}

// this will just be manual for now
void GetResumeInfo(std::vector<Person> &brothers)
{
    std::string output;
    ReadResumes(brothers, output);

    std::vector<std::string> resumeEntries = split(Strip(output), "$/|/$");

    // just check that there are equal number of brothers and entries (unlikely error but could be bad if not checked for)
    if (resumeEntries.size() != brothers.size())
    {
        std::cerr << "There are not the same number of brothers as there are resume entries. " << brothers.size() << " brothers and " << resumeEntries.size() << " resume entries" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < resumeEntries.size(); i++)
    {
        if (Strip(resumeEntries[i]) == "")
        {
            // this will need to do a manual entry
            ManualResumeInput(brothers[i]);
            continue;
        }

        ParseResumeEntry(brothers[i], resumeEntries[i]);
    }
        /*
        int selection;
        while (true)
        {
            std::cin >> selection;
            if (!std::cin.fail() && (selection >= 0 && selection <= 4))
            {
                if (selection == 2)
                {
                    formattedDate = FormatDate()
                    continue;
                }
                else if (selection > 0 && selection <= 4)
                {
                    jobEntry[selection] = 
                    continue;
                }
                break;
            }
            else
            {
                std::cout << "Incorrect number of entries please type 0, 1, 2, 3, 4" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }*/

}
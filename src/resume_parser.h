#pragma once

#include <unordered_map>
#include <iostream>

#include "person.h"
#include "functions.h"

///*std::vector <std::unordered_map<std::string, std::string>>*/void ReadResume(std::vector<std::string> args)
//{
//	std::string output = EXEC("executables\\pdf-reader.exe -dir");
//	std::cout << output << std::endl;
//}

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


// this will just be manual for now
void GetResumeInfo(Person &person)
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
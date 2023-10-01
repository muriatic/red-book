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
        std::cout << "Enter date range separated by semicolon (e.g. 2015 - 2020 would be entered as 2015;2020): " << std::endl;
        
        return FormatDate(ReadLine());
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
    for (int i = 0; i < numberOfEntries; i++)
    {
        std::map<std::string, std::string> jobEntryMap;
        std::cout << "Enter company name: " << std::endl;

        jobEntryMap.insert({"company", ReadLine()});

        std::cout << "Enter date range separated by semicolon (e.g. 2015 - 2020 would be entered as 2015;2020): " << std::endl;

        std::string formattedDate = FormatDate(ReadLine());

        jobEntryMap.insert({ "date", formattedDate });

        std::cout << "Enter Job title: " << std::endl;
        jobEntryMap.insert({ "position", ReadLine() });

        std::cout << "Enter location: " << std::endl;
        jobEntryMap.insert({ "location", ReadLine() });

        std::cout << "Confirm that the following information is correct:" << std::endl;
        std::cout << "(0) Nothing is wrong" << std::endl;
        std::cout << "(1) Company Name: " << jobEntryMap.at("company") << std::endl;
        std::cout << "(2) Date range: " << jobEntryMap.at("date") << std::endl;
        std::cout << "(3) Job title: " << jobEntryMap.at("position") << std::endl;
        std::cout << "(4) Location: " << jobEntryMap.at("location") << std::endl;

        person.parsedResumeInfo.push_back(jobEntryMap);
    }
}

float getDateLikelihood(std::string line)
{
    std::vector<std::string> months = { "january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december", "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };
    std::vector<std::string> years = { "january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december", "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

    std::regex rgx("\w+");
    std::smatch match;

    std::vector<std::string> words;

    int is_date_related = 0;
    int total = 0;

    while (std::regex_search(line, match, rgx)) {
        std::string word = match.str(0);

        // suffix to find the rest of the string.
        line = match.suffix().str();

        total++;

        if (IndexOf(months, ToLower(word)) != -1)
        {
            is_date_related++;
            continue;
        }
        if (IndexOf(years, ToLower(word)) != -1)
        {
            is_date_related++;
            continue;
        }
    }

    return ((float)is_date_related / (float)total);
}

float getLocationLikelihood(std::string line)
{
    std::regex rgx("(\w+)\s*,\s*(\w+)");
    std::smatch match;

    std::vector<std::string> states = { "alabama", "alaska", "arizona", "arkansas", "california", "colorado", "connecticut", "delaware", "florida", "georgia", "hawaii", "idaho", "illinois", "indiana", "iowa", "kansas", "kentucky", "louisiana", "maine", "maryland", "massachusetts", "michigan", "minnesota", "mississippi", "missouri", "montana", "nebraska", "nevada", "new hampshire", "new jersey", "new mexico", "new york", "north carolina", "north dakota", "ohio", "oklahoma", "oregon", "pennsylvania", "rhode island", "south carolina", "south dakota", "tennessee", "texas", "utah", "vermont", "virginia", "washington", "west virginia", "wisconsin", "wyoming", "district of columbia", "american samoa", "guam", "northern mariana islands", "puerto rico", "united states minor outlying islands", "u.s. virgin islands", "al", "ak", "az", "ar", "ca", "co", "ct", "de", "fl", "ga", "hi", "id", "il", "in", "ia", "ks", "ky", "la", "me", "md", "ma", "mi", "mn", "ms", "mo", "mt", "ne", "nv", "nh", "nj", "nm", "ny", "nc", "nd", "oh", "ok", "or", "pa", "ri", "sc", "sd", "tn", "tx", "ut", "vt", "va", "wa", "wv", "wi", "wy", "dc", "as", "gu", "mp", "pr", "um", "vi" };

    if (std::regex_search(line, match, rgx))
    {
        if (match.size() == 3)
        {
            if (IndexOf(states, ToLower(match[2].str())) != -1)
            {
                return 1.0f;
            }
        }
    }

    
    std::regex words_rgx("\w+");
    std::smatch wordsMatch;

    int is_location_related = 0;
    int total = 0;

    while (std::regex_search(line, wordsMatch, words_rgx)) {
        std::string word = wordsMatch.str(0);

        // suffix to find the rest of the string.
        line = wordsMatch.suffix().str();

        total++;

        if (IndexOf(states, ToLower(word)) != -1)
        {
            is_location_related++;
            continue;
        }
    }

    return ((float)is_location_related / (float)total);
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

    std::vector<std::map<std::string, std::string>> combinedMaps;

    bool logical = true;
    for (auto m : mmap)
    {
        std::vector<std::string> sub_list;
        for (int i = 0; i < m.second.size(); i++)
        {
            float likelihoodPCT = 0.5;
            if (getDateLikelihood(m.second[i]) >= likelihoodPCT)
            {
                sub_list.push_back("date");
                
                if (i != 1)
                {
                    logical = false;
                }
            }

            else if (getLocationLikelihood(m.second[i]) >= likelihoodPCT)
            {
                sub_list.push_back("location");

                if (i != 3)
                {
                    logical = false;
                }
            }

            else if (logical && i == 0)
            {
                sub_list.push_back("company");
            }

            else if (logical && i == 2)
            {
                sub_list.push_back("position");
            }
        }

        std::map<std::string, std::string> tempMap;
        // quickly iterate through sub_list
        for (int i = 0; i < sub_list.size(); i++)
        {
            tempMap.insert({ sub_list[i], m.second[i]});
        }
        combinedMaps.push_back(tempMap);
    }

    person.parsedResumeInfo = combinedMaps;
}

// this will just be manual for now
void GetResumeInfo(std::vector<Person> &brothers)
{
    //std::string output;
    //ReadResumes(brothers, output);

    //std::vector<std::string> resumeEntries = split(Strip(output), "$/|/$");

    // just check that there are equal number of brothers and entries (unlikely error but could be bad if not checked for)
    /*if (resumeEntries.size() != brothers.size())
    {
        std::cerr << "There are not the same number of brothers as there are resume entries. " << brothers.size() << " brothers and " << resumeEntries.size() << " resume entries" << std::endl;
        exit(EXIT_FAILURE);
    }*/

    // just manually go through brothers
    for (int i = 0; i < brothers.size(); i++)
    {
        ManualResumeInput(brothers[i]);
        //if (Strip(resumeEntries[i]) == "")
        //{
        //    // this will need to do a manual entry
        //    ManualResumeInput(brothers[i]);
        //    continue;
        //}

        //ParseResumeEntry(brothers[i], resumeEntries[i]);
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
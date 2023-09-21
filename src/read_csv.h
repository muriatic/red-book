#ifndef READ_CSV_H
#define READ_CSV_H

#include <fstream>
#include <string>
#include <sstream>
#include <optional>
#include <vector>
#include <string>
#include <filesystem>

#include "vector_functions.h"


// useful suffix checker, might be beneficial to put in another file
bool has_suffix(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}



struct Row
{
	std::vector<std::string> row {};
	std::vector<std::string> columnNames {};

	std::string getValue(std::string columnName)
	{
		int columnIDX = IndexOf(columnNames, columnName);

		if (columnIDX == -1)
		{
			std::cerr << "You are trying to access a column that does not exist" << std::endl;
			exit(EXIT_FAILURE);
		}

		return row.at(columnIDX);
	}
};

struct Column
{
	std::vector<char> col {};
};

class CSV
{
public:
	CSV(std::string fileName)
	{
		m_fileName = fileName;
		readCSV();
		createCSVMatrix();
		rowCount = rows.size();
		columnNames = rows[0].row;
	}
	
	// maybe not the best algorithm but oh well
	std::vector<std::string> getColumn(std::string title)
	{
		std::vector<std::string> column;
		
		// search first row
		int columnIDX = IndexOf(rows[0].row, title);

		if (columnIDX == -1)
		{
			std::cerr << "That particular column: " << title << " was not found." << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < rows.size(); i++)
		{
			column.push_back(rows[i].row[columnIDX]);
		}
		
		return column;
	}

	std::vector<std::string> getColumn(unsigned int index)
	{
		std::vector<std::string> column;

		if (index >= rows[0].row.size())
		{
			std::cerr << "That index is trying to access data outside the size of this array" << std::endl;
			exit(EXIT_FAILURE);
		}

		// search first row
		int columnIDX = index;

		for (int i = 0; i < rows.size(); i++)
		{
			column.push_back(rows[i].row[columnIDX]);
		}

		return column;
	}

	Row getRow(unsigned int index)
	{
		if (index >= rows.size())
		{
			std::cerr << "That index is trying to access data outside the size of this array" << std::endl;
			exit(EXIT_FAILURE);
		}

		return Row{ .row = rows[index].row, .columnNames = rows[0].row };
	}

	int rowCount;
	std::vector <std::string> columnNames;
private:
	std::string m_fileContents;
	std::string m_fileName;
	std::vector<Row> rows;
	size_t m_index = 0;
	int quotes : 1 = 0;
	void readCSV()
	{
		// check if the file exists first
		if (std::filesystem::exists(m_fileName) != 1 || !has_suffix(m_fileName, ".csv"))
		{
			std::cerr << "File: " << m_fileName << " does not exist or is not a CSV file." << std::endl;
			exit(EXIT_FAILURE);
		}

		std::stringstream contents_stream;
		std::fstream input(m_fileName, std::ios::in);
		contents_stream << input.rdbuf();
		m_fileContents = contents_stream.str();
	}
	void createCSVMatrix()
	{
		std::vector<std::string> row_buffer;
		std::vector<char> col_buffer;
		while (Peek().has_value())
		{
			if (Peek().value() == '\n')
			{
				Consume();
				// convert anything in the column buffer and add it to the row buffer
				row_buffer.push_back(std::string(col_buffer.begin(), col_buffer.end()));
				rows.push_back({ .row = row_buffer});
				col_buffer.clear();
				row_buffer.clear();
				continue;
			}
			else if (Peek().value() == ',')
			{
				Consume();
				row_buffer.push_back(std::string (col_buffer.begin(), col_buffer.end()));
				col_buffer.clear();
			}
			// quote string 
			//@ rules:
			// if comma in entry, quotes around it e.g. <sth, sth, sth> -> "sth, sth, sth"  
			// if there is at least one quote, whole string will be surrounded by quotes and each quote will be replaced with "" 
			// e.g. <"sth"> -> """sth"""; <s"t"h> -> "s""t""h"; <"sth> -> """sth"

			else if (Peek().value() == '"')
			{
				Consume();
				quotes++;
				while (Peek().has_value() && (quotes != 0 || (Peek().value() != ',' && Peek().value() != '\n')))
				{
					if (Peek().value() == '"')
					{
						Consume();
						quotes++;
						// we started with one quote so we know there's at least one
						unsigned int quote_count = 1;
						while (Peek().has_value() && Peek().value() == '"')
						{
							quotes++;
							Consume();
							quote_count++;
						}

						// honestly look at the above for help
						int remainder = quote_count % 3;
						int quotient = quote_count / 3;

						unsigned int number_of_quotes = 0;

						number_of_quotes += quotient;

						number_of_quotes += (remainder == 2) ? 1 : 0;

						// push_back " number of quotes times
						for (int i = 0; i < number_of_quotes; i++)
						{
							col_buffer.push_back('"');
						}

						quote_count = 0;
						number_of_quotes = 0;

						continue;
					}

					col_buffer.push_back(Consume());
				}
			}

			else 
			{
				col_buffer.push_back(Consume());
			}
		}
	}

	[[nodiscard]] std::optional <char> Peek(int offset = 0) const
	{
		if (m_index + offset >= m_fileContents.length())
		{
			return {};
		}
		return m_fileContents.at(m_index + offset);
	}

	// gets character at current index and increments (using post increment is not ideal due to readability but it simplifies the code and performance)
	char Consume()
	{
		return m_fileContents.at(m_index++);
	}
};

#endif
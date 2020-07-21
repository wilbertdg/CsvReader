#include "CsvReader.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>

CsvReader::CsvReader(const char* file) : fileName(file) { this->loadFile(file); }

void CsvReader::loadFile(const char* file)
{
	fileName = file;
	
	// If object has already been initialized, clear
	if (!fieldIndex.empty())
	{
		fieldIndex.clear();
		indexField.clear();
		content.clear();
	}

	std::string line;
	std::ifstream cvsFile;
	cvsFile.open(fileName);
	if (!cvsFile.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}

	// Put first row into the fieldIndex and indexField
	if (std::getline(cvsFile, line))
	{
		int index = 0;
		size_t start;
		size_t end = 0;
		while ((start = line.find_first_not_of(",", end)) != std::string::npos)
		{
			end = line.find(",", start);
			fieldIndex.insert(make_pair(line.substr(start, end - start), index));
			indexField.insert(make_pair(index, line.substr(start, end - start)));
			++index;
		}
	}

	// Put items into vector
	while (std::getline(cvsFile, line))
	{
		size_t start = 0;
		size_t end = 0;
		std::vector<std::string> v;
		for (size_t i = 0; i < indexField.size(); ++i)
		{
			if ((start = line.find_first_not_of(",", end)) != std::string::npos)
			{
				end = line.find(",", start);
				v.push_back(line.substr(start, end - start));
			}
			else
			{
				v.push_back("");
			}
		}
		content.push_back(v);
	}

	if (doubleQuotes)
	{
		trim('\"');
	}
}

std::vector<std::vector<std::string>> CsvReader::items()
{
	return content;
}

int CsvReader::fieldToIndex(std::string field)
{
	return fieldIndex[field];
}

std::string CsvReader::indexToField(int index)
{
	return indexField[index];
}

size_t CsvReader::fieldCount()
{
	return indexField.size();
}

void CsvReader::print()
{
	size_t largestSize = 0;

	// Find longest string to determine width
	for (auto &i : indexField)
	{
		if (largestSize < i.second.size())
		{
			largestSize = i.second.size();
		}
	}
	for (auto &i : content)
	{
		for (auto &j : i)
		{
			if (largestSize < j.size())
			{
				largestSize = j.size();
			}
		}
	}

	// Print
	for (auto &i : indexField)
	{
		std::cout << std::setw(largestSize + 3) << i.second;
	}

	std::cout << std::endl << std::endl;

	for (auto &i : content)
	{
		for (auto &j : i)
		{
			std::cout << std::setw(largestSize + 3) << j;
		}
		std::cout << std::endl;
	}
}

void CsvReader::trim(const char symbol)
{
	for (auto i : indexField)
	{
		while (i.second.front() == symbol)
		{
			i.second.erase(0, 1);
		}
		while (i.second.back() == symbol)
		{
			i.second.erase(i.second.size() - 1);
		}
	}

	for (auto i : content)
	{
		for (auto j : i)
		{
			while (j.front() == symbol)
			{
				j.erase(0, 1);
			}
			while (j.back() == symbol)
			{
				j.erase(j.size() - 1);
			}
		}
	}
}
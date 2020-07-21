#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

class CsvReader
{
private:
	bool doubleQuotes;
	std::string fileName;
	std::vector<std::vector<std::string>> content;
	std::unordered_map<std::string, int> fieldIndex;
	std::unordered_map<int, std::string> indexField;

public:
	CsvReader(const char* file);
	void loadFile(const char* file);
	std::vector<std::vector<std::string>> items();
	int fieldToIndex(std::string field);
	std::string indexToField(int index);
	size_t fieldCount();
	void print();
	void trim(const char symbol);
};
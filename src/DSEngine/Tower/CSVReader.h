#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


enum CardType {
	Intern = 0,
	Senior
};
enum Color {
	Blue = 1,
	Yellow = 10,
	Red = 100
};

struct TaskCardData
{
	CardType type;
	int Score;
	std::vector<int> pattern;
	std::vector<Color> color;
};

class CSVReader {
public:
	std::vector<TaskCardData> ReadFromFile(std::string _file);

private:
	void tokenize(std::string const &str, const char delim,
		std::vector<std::string> &out);
};
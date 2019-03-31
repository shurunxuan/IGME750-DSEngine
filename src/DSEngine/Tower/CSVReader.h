#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum CardType {
	Intern = 0,
	Senior
};
enum Color {
	Blue,
	Yellow,
	Red
};

struct TaskCardData
{
	CardType type;
	int Score;
	vector<int> pattern;
	vector<Color> color;
};

class CSVReader {
public:
	vector<TaskCardData> ReadFromFile(string _file);

private:
	void tokenize(std::string const &str, const char delim,
		std::vector<std::string> &out);
};
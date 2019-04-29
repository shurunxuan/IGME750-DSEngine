#include "TowerGameApp.h"
#include "CSVReader.h"
#include "UnlitMaterial.h"

using namespace std;

vector<TaskCardData> CSVReader::ReadFromFile(string _file) {
	vector<TaskCardData> data;

	ifstream ip(_file);
	if (!ip.is_open()) LOG_TRACE << "ERROR: File Open";

	string tp;
	string sc;
	string pat;
	string col;

	while (ip.good()) {
		TaskCardData card;
		getline(ip, tp, ',');
		getline(ip, sc, ',');
		getline(ip, pat, ',');
		getline(ip, col, '\n');

		//Type
		if (tp == "intern") card.type = Intern;			
		else if (tp == "senior") card.type = Senior;

		//Score
		card.Score = stoi(sc);

		//Pattern
		vector<int> pattern;
		for (size_t i = 0; i < pat.length(); i++)
			if (pat[i] != ';')
				pattern.push_back(static_cast<int>(pat[i]) - 48);				
		card.pattern = pattern;
		
		//Color
		const char delimiter = ';';
		vector<string> out;
		vector<Color> colors;
		//col = col.erase(col.find_last_of("\n"));
		tokenize(col, delimiter, out);
		for (size_t i = 0; i < out.size(); i++)
		{
			if (out[i] == "Blue" || out[i] == "Blue\n")
				colors.push_back(Blue);
			else if (out[i] == "Yellow" || out[i] == "Yellow\n")
				colors.push_back(Yellow);
			else if (out[i] == "Red" || out[i] == "Red\n")
				colors.push_back(Red);
		}
		card.color = colors;
		data.push_back(card);
	}
	ip.close();
	data.erase(data.begin()+data.size()-1);
	return data;
}

void CSVReader::tokenize(std::string const &str, const char delim,
	std::vector<std::string> &out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
	out.push_back(str.substr(end - start, end));
}

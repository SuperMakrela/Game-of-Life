#include "File.h"


using namespace std;

File::File()
{
}

File::~File()
{
}

void File::loadRules()
{
	ifstream file("defaultrules.txt");
	if (file.good())
	{
		string line;
		getline(file, line); //comment in file
		getline(file, line); //dead cell becomes alive rules

		istringstream ss(line);
		int num;
		while (ss >> num)
		{
			deadBecomesAlive.push_back(num);
		}
		getline(file, line); //comment in file
		getline(file, line); //alive cell dies rules

		ss.str(line);
		ss.clear();
		while (ss >> num)
		{
			aliveDies.push_back(num);
		}
	}
}
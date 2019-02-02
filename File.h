#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>


class File
{
public:
	std::vector <int> deadBecomesAlive;
	std::vector <int> aliveDies;
public:
	File();
	~File();
	void loadRules();
};
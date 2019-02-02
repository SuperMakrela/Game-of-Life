#pragma once
#include <vector> 
#include "File.h"

#define ALIVE 1
#define DEAD 0

class Cell
{
public:
	Cell();
	~Cell();
private:
	bool isAlive;
	int m_neighbours;
	File file;
public:
	int nextState();
	void update(bool state, int neighbours);
	bool rulesForDeadCell[9];
	bool rulesForAliveCell[9];
	void setDefaultRules();
};
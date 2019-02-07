#include "Cell.h"

Cell::Cell()
{
	file.loadRules();
	setDefaultRules();
}

Cell::~Cell()
{
}

bool Cell::nextState()
{
	if (rulesForDeadCell[m_neighbours] && !isAlive)  return ALIVE;
	else if (rulesForAliveCell[m_neighbours] && isAlive)  return DEAD;
	else return isAlive;
}

void Cell::update(bool state, int neighbours)
{
	isAlive = state;
	m_neighbours = neighbours;
}

void Cell::setDefaultRules()
{
	for (int i = 0; i < 9; i++)
	{
		rulesForDeadCell[i] = 0;
		rulesForAliveCell[i] = 0;
	}
	for (int i = 0; i < file.deadBecomesAlive.size(); i++)
	{
		rulesForDeadCell[file.deadBecomesAlive[i]] = true;
	}
	for (int i = 0; i < file.aliveDies.size(); i++)
	{
		rulesForAliveCell[file.aliveDies[i]] = true;
	}
}
#pragma once
#include <vector>
#include "Cell.h"
#include <time.h>       
#include <thread>         
#include <chrono>   

class Grid
{
private:
	int m_rows;
	int m_collumns;
	std::vector <char> m_grid; // test with bool
	std::vector <char> m_next;
public:
	Grid(int rows, int collumns);
	~Grid();
	void randomize();
	void clear();
	void generateNext();
	void wait(int time);
private:
	void updateGrid();
	int countNeighbours(int r, int c);
	int countNeighbours2(int r, int c);
public:
	bool getCellState(int cellNumber);
	void setCellState(int cellNumber, bool state);
	Cell cell;
	bool wrapEdges = true;
	unsigned int currentGeneration = 0;
	unsigned int cellsAlive;
};
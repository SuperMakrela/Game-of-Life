#include "Grid.h"   


Grid::Grid(int rows, int collumns) : m_rows(rows), m_collumns(collumns)
{
	m_grid.resize(m_rows*m_collumns);
	m_next.resize(m_rows*m_collumns);
	srand((unsigned int)time(NULL));
}

Grid::~Grid()
{
}

void Grid::randomize()
{
	currentGeneration = 0;
	cellsAlive = 0;
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_collumns; c++)
		{
			m_grid[r*m_collumns + c] = rand() & 1;
			cellsAlive += m_grid[r*m_collumns + c];
		}
	}
}

void Grid::clear()
{
	fill(m_grid.begin(), m_grid.end(), 0);
	cellsAlive = 0;
	currentGeneration = 0;
}

void Grid::generateNext()
{
	cellsAlive = 0;
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_collumns; c++)
		{
			int neighbours;
			if (wrapEdges) neighbours = countNeighbours(r, c);
			else neighbours = countNeighbours2(r, c);
			cell.update(m_grid[r * m_collumns + c], neighbours);
			m_next[r * m_collumns + c] = cell.nextState();
			cellsAlive += cell.nextState();
		}
	}
	currentGeneration++;
	Grid::updateGrid();
}

void Grid::updateGrid()
{
	m_grid.swap(m_next);
}

bool Grid::getCellState(int cellNumber)
{
	return m_grid[cellNumber];
}

void Grid::setCellState(int cellNumber, bool state)
{
	m_grid[cellNumber] = state;

}

void Grid::wait(int time)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

int Grid::countNeighbours(int r, int c) {
	int neighbours = 0;
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			int columnOfNeighbour = (c + x + m_collumns) % m_collumns;
			int rowOfNeighbour = (r + y + m_rows) % m_rows;
			neighbours += m_grid[rowOfNeighbour*m_collumns + columnOfNeighbour];
		}
	}
	neighbours -= m_grid[r * m_collumns + c];
	return neighbours;
}

int Grid::countNeighbours2(int r, int c) {
	int neighbours = 0;
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			int columnOfNeighbour = c + x;
			int rowOfNeighbour = r + y;
			if (columnOfNeighbour >= 0 && columnOfNeighbour < m_collumns && rowOfNeighbour >= 0 && rowOfNeighbour < m_rows)
			{
				neighbours += m_grid[rowOfNeighbour*m_collumns + columnOfNeighbour];
			}
		}
	}
	neighbours -= m_grid[r * m_collumns + c];
	return neighbours;
}
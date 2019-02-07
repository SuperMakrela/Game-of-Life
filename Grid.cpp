#include "Grid.h"   


Grid::Grid(int rows, int collumns) : m_rows(rows), m_columns(collumns)
{
	m_grid.resize(m_rows*m_columns);
	m_next.resize(m_rows*m_columns);
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
		for (int c = 0; c < m_columns; c++)
		{
			m_grid[r*m_columns + c] = rand() & 1;
			cellsAlive += m_grid[r*m_columns + c];
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
		for (int c = 0; c < m_columns; c++)
		{
			int neighbours;
			if (wrapEdges) neighbours = countNeighbours_WrappedEdges(r, c);
			else neighbours = countNeighbours_NormalEdges(r, c);
			cell.update(m_grid[r * m_columns + c], neighbours);
			m_next[r * m_columns + c] = cell.nextState();
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

int Grid::countNeighbours_WrappedEdges(int r, int c) {
	int neighbours = -m_grid[r * m_columns + c];
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			neighbours += m_grid[((r + y + m_rows) % m_rows)*m_columns + ((c + x + m_columns) % m_columns)];
		}
	}
	return neighbours;
}

int Grid::countNeighbours_NormalEdges(int r, int c) {
	int neighbours = -m_grid[r * m_columns + c];
	int columnOfNeighbour, rowOfNeighbour;
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			columnOfNeighbour = c + x;
			rowOfNeighbour = r + y;
			if (columnOfNeighbour >= 0 && columnOfNeighbour < m_columns && rowOfNeighbour >= 0 && rowOfNeighbour < m_rows)
			{
				neighbours += m_grid[rowOfNeighbour*m_columns + columnOfNeighbour];
			}
		}
	}
	return neighbours;
}
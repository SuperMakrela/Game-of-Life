#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "windows.h"
#include <string>

class Game
{
private:
	const float cellSize;
	const float gapSize;
	const int rows, collumns;
public:
	Game(int windowSizeX, int windowSizeY);
	~Game();
	void run();
private:
	void processEvents();
	void update();
	void render();
	sf::RenderWindow window;
	sf::RectangleShape cellSquare;
	bool pauseGame = 0;
	Grid grid;
	const int delay[8] = { 20,40,60,100,150,250,350,500 };
	int currentDelayIndex = 3;
	sf::Font font;
	sf::Text text;
	sf::RectangleShape menuLine;
	sf::RectangleShape rulesSquare;
	const float menuHeight = rows * cellSize;
};
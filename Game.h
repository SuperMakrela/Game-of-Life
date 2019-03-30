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
	const int allRows, allColumns;
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
	bool isGamePaused = false;
	Grid grid;
	const int delay[8] = { 20,40,60,100,150,250,350,500 };
	int currentDelayIndex = 3;
	sf::Font font;
	sf::Text text;
	sf::RectangleShape menuLine;
	sf::RectangleShape rulesSquare;
	const float menuPosition = allRows * cellSize; //Pixels from top to start of menu bar
	float drawPosX;
	float drawPosY;
	void drawText(int size, std::string text, float posX, float posY,
		sf::Color color = sf::Color::White, sf::Text::Style style = sf::Text::Regular);
	void buttonSpace();
	void buttonR();
	void buttonC();
	void buttonS();
	void buttonE();
	void buttonLeft();
	void buttonRight();
	void buttonD();
	void gameExit();
};
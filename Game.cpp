#include "Game.h"


Game::Game(int windowSizeX, int windowSizeY) :
	window(sf::VideoMode(windowSizeX, windowSizeY), "Game of Life", sf::Style::Titlebar | sf::Style::Close),
	cellSize(10), gapSize(1), allRows(static_cast <int> (windowSizeY / cellSize) - 10), allColumns(static_cast <int> (windowSizeX / cellSize)),
	cellSquare{ sf::Vector2f(cellSize - gapSize, cellSize - gapSize) }, grid(allRows, allColumns), rulesSquare{ sf::Vector2f(19.f,19.f) }
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	window.setFramerateLimit(60);
	cellSquare.setFillColor(sf::Color::Green);
	rulesSquare.setFillColor(sf::Color::Green);

	font.loadFromFile("OpenSans-Regular.ttf");
	text.setFont(font);

	menuLine.setSize(sf::Vector2f(static_cast <float> (windowSizeX), 2.f));
	menuLine.setFillColor(sf::Color::White);
}

Game::~Game()
{
}

void Game::run()
{
	grid.randomize();
	while (window.isOpen())
	{
		processEvents();
		render();
		if (isGamePaused) {
			grid.wait(100);
			continue;
		}
		update();
	}
}

void Game::update()
{
	grid.generateNext();
	grid.wait(delay[currentDelayIndex]);
}

void Game::render()
{
	window.clear(sf::Color::Black);

	for (int r = 0; r < allRows; r++)
	{
		drawPosY = r * cellSize;
		for (int c = 0; c < allColumns; c++)
		{
			if (grid.getCellState(r*allColumns + c)) {
				drawPosX = (r * allColumns + c) % allColumns * cellSize;
				cellSquare.setPosition(sf::Vector2f(drawPosX, drawPosY));
				window.draw(cellSquare);
			}
		}
		drawPosY += cellSize;
	}

	//Informations at the top of the menu
	menuLine.setPosition(0.f, menuPosition);
	window.draw(menuLine);
	menuLine.setPosition(0.f, menuPosition + 32);
	window.draw(menuLine);
	drawText(18, std::string("[Space] Pause\t[R] Randomize\t[C] Clear\t[S] Single step\t[E]\ Edge mode\t\[Left/Right arrow]\
 Speed\t[D] Default rules"), 10.f, menuPosition + 5, sf::Color::Magenta);
	drawText(24, std::string("RULES:"), 0.f, menuPosition + 35, sf::Color::White, sf::Text::Underlined);
	drawText(14, std::string("Number of neighbours:"), 150.f, menuPosition + 40);

	//Changing rules and color of coresponding square
	drawPosX = 150;
	drawPosY = menuPosition + 60;
	drawText(14, std::string("Alive dies:"), 0.f, drawPosY + 4);
	drawText(14, std::string("Dead becomes alive:"), 0.f, drawPosY + 24);
	for (int textRow = 0; textRow < 2; textRow++)
	{
		for (int i = 0; i < 9; i++)
		{
			rulesSquare.setFillColor(
				(textRow && grid.cell.rulesForDeadCell[i]) || (!textRow && grid.cell.rulesForAliveCell[i])
				? sf::Color::White : sf::Color::Black
			);

			rulesSquare.setPosition(sf::Vector2f(drawPosX + i * 20, drawPosY));
			window.draw(rulesSquare);
			drawText(10, std::to_string(i), drawPosX + 1 + i * 20, drawPosY, sf::Color::Black);
		}
		drawPosY += 20;
	}
	//Additional informations
	drawText(14, std::string("Delay: ").append(std::to_string(delay[currentDelayIndex])).append(" ms"), 400.f, menuPosition + 40);
	drawText(14, std::string("Edge mode: ").append(grid.wrapEdges ? "Wrapped" : "Normal"), 400.f, menuPosition + 60);
	drawText(14, std::string("Current generation: ").append(std::to_string(grid.currentGeneration)), 600.f, menuPosition + 40);
	drawText(14, std::string("Cells alive: ").append(std::to_string(grid.cellsAlive)), 600.f, menuPosition + 60);
	window.display();
}

void Game::drawText(int size, std::string message, float posX, float posY, sf::Color color, sf::Text::Style style)
{
	text.setCharacterSize(size);
	text.setString(message);
	text.setPosition(posX, posY);
	text.setFillColor(color);
	text.setStyle(style);
	window.draw(text);
}

void Game::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			gameExit();
			break;

		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{	//Inside grid space
				int msX = event.mouseButton.x;
				int msY = event.mouseButton.y;
				int column = static_cast <int>(msX / cellSize);
				int row = static_cast <int>(msY / cellSize);

				if (row < allRows && column < allColumns) {
					//Change the state and update the number of cells
					grid.setCellState(column + row * allColumns, !grid.getCellState(column + row * allColumns));
					if (grid.getCellState(column + row * allColumns)) grid.cellsAlive++;
					else grid.cellsAlive--;
				}
				else if (msX > 150 && msX < 330 && msY > menuPosition + 60 && msY < menuPosition + 100)
				{	//Change rules menu
					column = static_cast <int>(msX - 150) / 20;
					row = static_cast <int>(msY - menuPosition - 60) / 20;
					if (row) grid.cell.rulesForDeadCell[column] = !grid.cell.rulesForDeadCell[column];
					else  grid.cell.rulesForAliveCell[column] = !grid.cell.rulesForAliveCell[column];
				}
				//Menu buttons
				else if (msY > menuPosition + 10 && msY < menuPosition + 28) {
					if (msX > 10 && msX < 130)        buttonSpace();
					else if (msX > 150 && msX < 271)  buttonR();
					else if (msX > 292 && msX < 365)  buttonC();
					else if (msX > 385 && msX < 507)  buttonS();
					else if (msX > 528 && msX < 650)  buttonE();
					else if (msX > 671 && msX < 711)  buttonLeft();
					else if (msX > 720 && msX < 765)  buttonRight();
					else if (msX > 905 && msX < 1045) buttonD();
				}
			}
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Space:	buttonSpace();
				break;
			case sf::Keyboard::R:		buttonR();
				break;
			case sf::Keyboard::C:		buttonC();
				break;
			case sf::Keyboard::S:		buttonS();
				break;
			case sf::Keyboard::E:		buttonE();
				break;
			case sf::Keyboard::Left:	buttonLeft();
				break;
			case sf::Keyboard::Right:	buttonRight();
				break;
			case sf::Keyboard::D:		buttonD();
				break;
			case sf::Keyboard::Escape:	gameExit();
				break;
			}
		}
	}
}

void Game::buttonSpace() {
	isGamePaused = !isGamePaused;
	if (isGamePaused) cellSquare.setFillColor(sf::Color::Red);
	else cellSquare.setFillColor(sf::Color::Green);
}
void Game::buttonR() {
	grid.randomize();
	grid.wait(100);
}
void Game::buttonC() {
	grid.clear();
	grid.wait(100);
}
void Game::buttonS() {
	grid.generateNext();
	isGamePaused = true;
	cellSquare.setFillColor(sf::Color::Red);
}
void Game::buttonE() {
	grid.wrapEdges = !grid.wrapEdges;
}
void Game::buttonLeft() {
	if (currentDelayIndex < 7) currentDelayIndex++;
}
void Game::buttonRight() {
	if (currentDelayIndex < 7) currentDelayIndex--;
}
void Game::buttonD() {
	grid.cell.setDefaultRules();
}
void Game::gameExit() {
	::ShowWindow(GetConsoleWindow(), SW_RESTORE);
	window.close();
}
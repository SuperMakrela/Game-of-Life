#include "Game.h"


Game::Game(int windowSizeX, int windowSizeY) :
	window(sf::VideoMode(windowSizeX, windowSizeY), "Game of Life", sf::Style::Titlebar | sf::Style::Close),
	cellSize(10), gapSize(1), rows(static_cast <int> (windowSizeY / cellSize) - 10), collumns(static_cast <int> (windowSizeX / cellSize)),
	cellSquare{ sf::Vector2f(cellSize - gapSize, cellSize - gapSize) }, grid(rows, collumns), rulesSquare{ sf::Vector2f(19.f,19.f) }
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

		if (pauseGame) {
			grid.wait(100);
			continue;
		}

		update();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			::ShowWindow(GetConsoleWindow(), SW_RESTORE);
			window.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//grid input
				int collumn = static_cast <int>(event.mouseButton.x / cellSize);
				int row = static_cast <int>(event.mouseButton.y / cellSize);
				if (row < rows && collumn < collumns) {
					grid.setCellState(collumn + row * collumns, !grid.getCellState(collumn + row * collumns));
					if (!grid.getCellState(collumn + row * collumns)) grid.cellsAlive--;
					else grid.cellsAlive++;
				}
				else if (event.mouseButton.x > 150 && event.mouseButton.x < 330 && event.mouseButton.y > menuPosition + 60 && event.mouseButton.y < menuPosition + 100)
				{
					//menu input - mouse
					//changing rules
					collumn = static_cast <int>(event.mouseButton.x - 150) / 20;
					row = static_cast <int>(event.mouseButton.y - menuPosition - 60) / 20;
					if (row) grid.cell.rulesForDeadCell[collumn] = !grid.cell.rulesForDeadCell[collumn];
					else  grid.cell.rulesForAliveCell[collumn] = !grid.cell.rulesForAliveCell[collumn];
				}
				else if (event.mouseButton.y > menuPosition + 10 && event.mouseButton.y < menuPosition + 28) {
					if (event.mouseButton.x > 10 && event.mouseButton.x < 130)
					{
						//space
						pauseGame = !pauseGame;
						if (pauseGame) cellSquare.setFillColor(sf::Color::Red);
						else cellSquare.setFillColor(sf::Color::Green);
					}
					else if (event.mouseButton.x > 150 && event.mouseButton.x < 271)
					{
						//R
						grid.randomize();
						grid.wait(100);
					}
					else if (event.mouseButton.x > 292 && event.mouseButton.x < 365)
					{
						//C
						grid.clear();
						grid.wait(100);
					}
					else if (event.mouseButton.x > 385 && event.mouseButton.x < 507)
					{
						//S
						grid.generateNext();
						pauseGame = 1;
						cellSquare.setFillColor(sf::Color::Red);
					}
					else if (event.mouseButton.x > 528 && event.mouseButton.x < 650)
					{
						//E
						grid.wrapEdges = !grid.wrapEdges;
					}
					else if (event.mouseButton.x > 671 && event.mouseButton.x < 711)
					{
						//Left
						if (currentDelayIndex < 7) currentDelayIndex++;
					}
					else if (event.mouseButton.x > 720 && event.mouseButton.x < 765)
					{
						//Right
						if (currentDelayIndex > 0) currentDelayIndex--;
					}
					else if (event.mouseButton.x > 905 && event.mouseButton.x < 1045)
					{
						//D
						grid.cell.setDefaultRules();
					}
				}
			}
			break;
		case sf::Event::KeyPressed: //keyboard input
			switch (event.key.code)
			{
			case sf::Keyboard::C:
				grid.clear();
				grid.wait(100);
				break;
			case sf::Keyboard::R:
				grid.randomize();
				grid.wait(100);
				break;
			case sf::Keyboard::S:
				grid.generateNext();
				pauseGame = 1;
				cellSquare.setFillColor(sf::Color::Red);
				break;
			case sf::Keyboard::Space:
				pauseGame = !pauseGame;
				if (pauseGame) cellSquare.setFillColor(sf::Color::Red);
				else cellSquare.setFillColor(sf::Color::Green);
				break;
			case sf::Keyboard::Left:
				if (currentDelayIndex < 7) currentDelayIndex++;
				break;
			case sf::Keyboard::Right:
				if (currentDelayIndex > 0) currentDelayIndex--;
				break;
			case sf::Keyboard::E:
				grid.wrapEdges = !grid.wrapEdges;
				break;
			case sf::Keyboard::Escape:
				::ShowWindow(GetConsoleWindow(), SW_RESTORE);
				window.close();
				break;
			case sf::Keyboard::D:
				grid.cell.setDefaultRules();
				break;
			}
		}
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

	for (int r = 0; r < rows; r++)
	{
		drawPosY = r * cellSize;
		for (int c = 0; c < collumns; c++)
		{
			if (grid.getCellState(r*collumns + c)) {
				drawPosX = (r * collumns + c) % collumns * cellSize;
				cellSquare.setPosition(sf::Vector2f(drawPosX, drawPosY));
				window.draw(cellSquare);
			}
		}
		drawPosY += cellSize;
	}

	//top of the menu
	menuLine.setPosition(0.f, menuPosition);
	window.draw(menuLine);
	menuLine.setPosition(0.f, menuPosition + 32);
	window.draw(menuLine);

	drawText(std::string("[Space] Pause\t[R] Randomize\t[C] Clear\t[S] Single step\t[E]\ Edge mode\t\[Left/Right arrow]\
 Speed\t[D] Default rules"), 18, 10.f, menuPosition + 5, sf::Color::Magenta);
	drawText(std::string("RULES:"), 24, 0.f, menuPosition + 35, sf::Color::White, sf::Text::Underlined);
	drawText(std::string("Number of neighbours:"), 14, 150.f, menuPosition + 40);

	//rules
	drawPosX = 150;
	drawPosY = menuPosition + 60;
	drawText(std::string("Alive dies:"), 14, 0.f, drawPosY + 4);
	drawText(std::string("Dead becomes alive:"), 14, 0.f, drawPosY + 24);
	for (int textRow = 0; textRow < 2; textRow++)
	{
		for (int i = 0; i < 9; i++)
		{
			if ((textRow && grid.cell.rulesForDeadCell[i]) || (!textRow && grid.cell.rulesForAliveCell[i]))
				rulesSquare.setFillColor(sf::Color::White);
			else rulesSquare.setFillColor(sf::Color::Black);
			rulesSquare.setPosition(sf::Vector2f(drawPosX + i * 20, drawPosY));
			window.draw(rulesSquare);
			drawText(std::to_string(i), 10, drawPosX + 1 + i * 20, drawPosY, sf::Color::Black);
		}
		drawPosY += 20;
	}
	//info
	drawText(std::string("Delay: ").append(std::to_string(delay[currentDelayIndex])).append(" ms"), 14, 400.f, menuPosition + 40);
	drawText(std::string("Edge mode: ").append(grid.wrapEdges ? "Wrapped" : "Normal"), 14, 400.f, menuPosition + 60);
	drawText(std::string("Current generation: ").append(std::to_string(grid.currentGeneration)), 14, 600.f, menuPosition + 40);
	drawText(std::string("Cells alive: ").append(std::to_string(grid.cellsAlive)), 14, 600.f, menuPosition + 60);

	window.display();
}

void Game::drawText(std::string message, int size, float posX, float posY, sf::Color color, sf::Text::Style style)
{
	text.setString(message);
	text.setCharacterSize(size);
	text.setPosition(posX, posY);
	text.setFillColor(color);
	text.setStyle(style);
	window.draw(text);
}
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
				else if (event.mouseButton.x > 150 && event.mouseButton.x < 330 && event.mouseButton.y > menuHeight + 60 && event.mouseButton.y < menuHeight + 100)
				{
					//menu input - mouse
					//changing rules
					collumn = static_cast <int>(event.mouseButton.x - 150) / 20;
					row = static_cast <int>(event.mouseButton.y - menuHeight - 60) / 20;
					if (row) grid.cell.rulesForDeadCell[collumn] = !grid.cell.rulesForDeadCell[collumn];
					else  grid.cell.rulesForAliveCell[collumn] = !grid.cell.rulesForAliveCell[collumn];
				}
				else if (event.mouseButton.y > menuHeight + 10 && event.mouseButton.y < menuHeight + 28) {
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

	float drawPosition_x;
	float drawPosition_y = 0;

	for (int r = 0; r < rows; r++)
	{
		drawPosition_y = r * cellSize;
		for (int c = 0; c < collumns; c++)
		{
			if (grid.getCellState(r*collumns + c)) {
				drawPosition_x = (r * collumns + c) % collumns * cellSize;
				cellSquare.setPosition(sf::Vector2f(drawPosition_x, drawPosition_y));
				window.draw(cellSquare);
			}
		}
		drawPosition_y += cellSize;
	}

	//drawing top of the menu

	menuLine.setPosition(0.f, menuHeight);
	window.draw(menuLine);
	menuLine.setPosition(0.f, menuHeight + 32);
	window.draw(menuLine);

	text.setString("[Space] Pause\t[R] Randomize\t[C] Clear\t[S] Single step\t[E] Edge mode\t[Left/Right arrow] Speed\t[D] Default rules");
	text.setCharacterSize(18);
	text.setFillColor(sf::Color::Magenta);
	text.setPosition(10.f, menuHeight + 5);
	window.draw(text);

	text.setStyle(sf::Text::Underlined);
	text.setString("RULES:");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(0.f, menuHeight + 35);
	window.draw(text);
	text.setStyle(sf::Text::Regular);

	text.setString("Number of neighbours:");
	text.setCharacterSize(14);
	text.setPosition(150.f, menuHeight + 40);
	window.draw(text);

	//drawing rules
	drawPosition_x = 150;
	drawPosition_y = menuHeight + 60;

	text.setString("Alive dies:");
	for (int textRow = 0; textRow < 2; textRow++)
	{
		text.setCharacterSize(14);
		text.setFillColor(sf::Color::White);
		text.setPosition(0.f, drawPosition_y + 4);
		window.draw(text);

		text.setCharacterSize(10);
		text.setFillColor(sf::Color::Black);
		for (int i = 0; i < 9; i++)
		{
			if ((textRow && grid.cell.rulesForDeadCell[i]) || (!textRow && grid.cell.rulesForAliveCell[i]))
				rulesSquare.setFillColor(sf::Color::White);
			else rulesSquare.setFillColor(sf::Color::Black);

			rulesSquare.setPosition(sf::Vector2f(drawPosition_x + i * 20, drawPosition_y));
			window.draw(rulesSquare);
			text.setPosition(sf::Vector2f(drawPosition_x + i * 20 + 1, drawPosition_y));
			text.setString(static_cast <char>('0' + i));
			window.draw(text);
		}
		drawPosition_y += 20;
		text.setString("Dead becomes alive:");
	}

	//drawing info
	std::string s;
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(14);

	s = "Delay: ";
	s.append(std::to_string(delay[currentDelayIndex]));
	s.append(" ms");
	text.setString(s);
	text.setPosition(400.f, menuHeight + 40);
	window.draw(text);

	s = "Edge mode: ";
	if (grid.wrapEdges) s.append("Wrapped");
	else s.append("Normal");
	text.setString(s);
	text.setPosition(400.f, menuHeight + 60);
	window.draw(text);

	s = "Current generation: ";
	s.append(std::to_string(grid.currentGeneration));
	text.setString(s);
	text.setPosition(600.f, menuHeight + 40);
	window.draw(text);

	s = "Cells alive: ";
	s.append(std::to_string(grid.cellsAlive));
	text.setString(s);
	text.setPosition(600.f, menuHeight + 60);
	window.draw(text);

	window.display();
}
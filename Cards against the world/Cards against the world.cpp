// Cards against the world.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include "Settings.h"
#include "menu.h"
#include "Sounds.h"
#include <iostream>
#include "Deck.h"
Settings setting;
Deck deck;
int main()
{
	try
	{
		sf::ContextSettings set;
		set.antialiasingLevel = 8;
		std::string version = "Beta 0.1";
		auto state = st::mainMenu;
		sf::Vector2i defaultSize(1920, 1080);
		sf::RenderWindow window(sf::VideoMode(defaultSize.x, defaultSize.y), "Sylio " + version, sf::Style::Fullscreen, set);
		setting.xScale = double(window.getSize().x) / defaultSize.x;
		setting.yScale = double(window.getSize().y) / defaultSize.y;

		window.clear(sf::Color::Black);
		Menu Menu_(window, version);
		sf::Image pointnerIm;
		if (!pointnerIm.loadFromFile("PNG/pointner2.png"))
			throw std::exception("pointner png file missing");
		sf::Cursor pointner;
		if (!pointner.loadFromPixels(pointnerIm.getPixelsPtr(), sf::Vector2u(30, 31), sf::Vector2u(6, 1)))
			throw std::exception("loading pointner error");
		window.setMouseCursor(pointner);
		window.setMouseCursorVisible(false);

		window.setActive(true);
		window.setMouseCursorVisible(true);

		while (true)
		{
			switch (state)
			{
			case st::mainMenu:
				window.setMouseCursorVisible(true);
				state = Menu_.mainMenuUpdate();
				break;
			case st::choseGameMode:
				window.setMouseCursorVisible(true);
				state = Menu_.choseGameMode();
				break;
			case st::connectToLobby:
				window.setMouseCursorVisible(true);
				state = Menu_.ConnectToLobby();
				break;
			case st::settings:
				state = Menu_.test();
				break;
			case st::quit:
				setting.save();
				window.close();
				return 0;
				break;
			default:

				break;
			}
		}
		return 0;
	}
	catch (std::exception & except)
	{
		std::cerr << except.what();
		exit(-1);
	}
	catch (...)
	{
		std::cerr << "unknown error appers";
		exit(-1);
	}
}
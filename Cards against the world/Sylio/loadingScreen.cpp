#include "loadingScreen.h"
#include "Settings.h"



extern Settings setting;

loadingScreen::loadingScreen(sf::RenderWindow& win, std::atomic_flag& fl): window(win), flag(fl)
{
	if (!font.loadFromFile("../Font/kenvector_future.ttf"))
		throw std::exception("font file missing");
	loading.setFillColor(sf::Color::White);
	loading.setString("Loading ...");
	loading.setCharacterSize(80);
	loading.setFont(font);
	loading.setPosition(window.getSize().x / 2 - loading.getGlobalBounds().width/2, window.getSize().y / 2 - loading.getGlobalBounds().height/2);	
	loading.setScale(setting.xScale, setting.yScale);
}

loadingScreen::~loadingScreen()
{
}

void loadingScreen::operator()()
{
	std::string str = "Loading ";
	std::string dots = "";
	loading.setString(str);
	globclock.restart();
	clock.restart();
	window.setActive(true);
	window.draw(loading);
	window.display();
	while (true) {
		while (clock.getElapsedTime().asSeconds() < 0.3);
		if (globclock.getElapsedTime().asSeconds() > 0 && !flag.test_and_set())
			break;
		else if (dots.size() < 3)
			dots += ".";
		else
			dots = "";
		clock.restart();
		loading.setString(str + dots);
		window.clear(sf::Color::Black);
		window.draw(loading);
		window.display();
	}
	window.setActive(false);
}
#pragma once
#include<SFML/Graphics.hpp>
#include <atomic>
class loadingScreen
{
private:
	sf::RenderWindow& window;
	std::atomic_flag& flag;
	sf::Font font;
	sf::Clock clock;
	sf::Clock globclock;
	sf::Text loading;
public:
	loadingScreen(sf::RenderWindow& win, std::atomic_flag& fl);
	~loadingScreen();
	void operator()();
};


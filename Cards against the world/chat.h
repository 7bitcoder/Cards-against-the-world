#pragma once
#include "inputText.h"
class textContainer;
class chat
{
private:
	sf::RenderWindow& window;
	sf::RectangleShape box;
	sf::RectangleShape inpuBox;
	sf::RectangleShape rightSide;
	sf::RectangleShape scroll;
	inputText textBar;
	textContainer continer;
public:
	chat(sf::RenderWindow& win);
	void setPosision(int x, int y);
	~chat();
};


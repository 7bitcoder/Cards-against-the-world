#pragma once
#include "inputText.h"
class textContainer;
class chat
{
private:
	std::vector<sf::Text> container;
	const std::vector<sf::Text*> visible;
	sf::RenderWindow& window;
	sf::RectangleShape box;
	sf::RectangleShape inputBox;
	sf::RectangleShape rightSide;
	sf::RectangleShape scroll;
	inputText textBar;
	std::u32string input;

public:
	void checkState();
	bool addChar(sf::Event::KeyEvent h);//inputteext wrappers
	bool function(bool clear = false);
	void draw();
	void operator<<(std::u32string string);//text compute
	void computeText();//divide text to fit in container
	chat(sf::RenderWindow& win, sf::SoundBuffer& click_, int charLimit, int charSize, int chatlines);
	void setValues(sf::Vector2i position, int charSize, int chatlines, int widen);
	void setPosision(int x, int y);
	~chat();
};


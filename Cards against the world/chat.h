#pragma once
#include "inputText.h"
#include "Slider.h"
#include <ctime>
class textContainer;
class chat
{
private:
	time_t rawtime;
	int charSize_;
	int chatLines;
	bool scrollable;
	std::vector<sf::Vector2f> containerPositions;
	std::vector<sf::Text> container;
	std::vector<int> visible;
	sf::RenderWindow& window;
	sf::RectangleShape box;
	sf::RectangleShape inputBox;
	sf::RectangleShape rightSide;
	Slider scroll;
	sf::RectangleShape inputRightTerm;
	sf::RectangleShape inputLeftTerm;
	sf::Font & font;
	inputText textBar;
	sf::Text test;
	sf::String textToSend;
	bool moveUp();
	bool moveDown();
	void reset();
	void setContainerPositions();
	void computeText(sf::String& string, sf::Color col);//divide text to fit in container
	void addTime(sf::String& string);
	void computeScrollBarSize();
	void move(int delta);
public:
	chat(sf::RenderWindow& win, sf::SoundBuffer& click_, int charLimit, int chatlines, sf::Font& font_);
	void setValues(int charSize, int widen);
	void checkState();
	void checkSideBarState();
	bool addChar(sf::Event::KeyEvent h);//inputteext wrappers
	bool function() { return textBar.function(); }
	void draw();
	void scrolled(int delta);
	std::u32string getText() { std::u32string tmp; for (auto x : textToSend) tmp += x; return tmp; }
	void send(sf::String string, sf::Color col) { sf::String input = string; computeText(input, col); };//text compute
	void setPosition(sf::Vector2f position);
	void operator<<(std::u32string string) {};//text compute
	~chat();
};


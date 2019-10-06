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
	bool moveUp();
	bool moveDown();
	void reset();
	void setContainerPositions();
	void computeText(sf::String& string);//divide text to fit in container
	void addTime(sf::String& string);
	void computeScrollBarSize();
	void move(int delta);
public:
	chat(sf::RenderWindow& win, sf::SoundBuffer& click_, int charLimit, int chatlines, sf::Font& font_);
	void setValues(sf::Vector2f position, int charSize, int widen);
	void checkState();
	void checkSideBarState();
	bool addChar(sf::Event::KeyEvent h);//inputteext wrappers
	bool function() { return textBar.function(); }
	void draw();
	void scrolled(int delta);
	void operator<<(sf::String string) { sf::String input = string; computeText(input); };//text compute
	void operator<<(std::u32string string) {};//text compute
	~chat();
};


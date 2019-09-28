#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include"Settings.h"
extern Settings setting;
class inputText
{
private:
	enum buttonState { isPressed, isNotPressed };
	enum positionState { isOn, isNotOn };
	sf::Text textOutput;
	sf::Texture& box;
	sf::RectangleShape blueMark;//blue mark when trying to copy;
	sf::RectangleShape coursor;
	sf::Sprite spriteBox;
	sf::RenderWindow& window;
	std::size_t coursorPosition;
	std::size_t coursorLastPosition;
	int limit;
	sf::Sound click;
	bool focused;
	bool blink;
	bool clicked;
	bool activated;
	sf::Clock timer;
	sf::Clock slowClock;
	sf::String lastChar;
	sf::String text;
	positionState positionSt;
	positionState lastPositionSt;
	buttonState buttonSt;
	buttonState lastButtonSt;
	bool isOnButton();
	void checkCoursorPosition();
	void setCoursorPosition(std::size_t i);
	void checkBlink();
	void mark();
public:
	inputText(sf::RenderWindow& win, sf::Texture& box_, sf::SoundBuffer& click, int charLimit);
	bool function(bool clear = false);
	void checkState();
	bool addChar(sf::String t);
	void setString(std::string y) { text = y; textOutput.setString(y); }
	void draw() { window.draw(spriteBox); if (focused) window.draw(blueMark) ; window.draw(textOutput); if (blink) window.draw(coursor); }
	void setPosition(int x, int y);
	void setTextPosition(int x, int y) { textOutput.setPosition(x, y);  textOutput.setScale(setting.xScale, setting.yScale); };
	void setColor(sf::Color x = sf::Color::Black) { textOutput.setFillColor(x); }
	void setFont(sf::Font& x) { textOutput.setFont(x); }
	void setSize(int x) { textOutput.setCharacterSize(x); coursor.setSize(sf::Vector2f(2, x)); coursor.setFillColor(sf::Color(128,128,128,255)); blueMark.setSize(sf::Vector2f(0, x)); blueMark.setFillColor(sf::Color::Blue);
	}
	sf::String& getText() { return text; }
	void clear() { text.clear(); textOutput.setString(""); }
	void setScale(double x, double y) { spriteBox.setScale(x, y); }
	void setSoundVolume(double vol) { click.setVolume(vol * 100); }
	~inputText();
};


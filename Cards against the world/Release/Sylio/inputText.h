#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include"Settings.h"
extern Settings setting;
class inputText
{
private:
	sf::Text textOutput;
	enum buttonState { isPressed, isNotPressed };
	enum positionState { isOn, isNotOn };
	sf::Texture& boxOn;
	sf::Texture& boxOff;
	sf::Sprite spriteBox;
	sf::RenderWindow& window;
	positionState positionSt;
	positionState lastPositionSt;
	buttonState buttonSt;
	buttonState lastButtonSt;
	sf::Sound click;
	bool clicked;
	bool activated;
	bool focuse;
	std::string text;
	bool isOnButton();

public:
	inputText(sf::RenderWindow& win, sf::Texture& box_, sf::Texture& boxOff_, sf::SoundBuffer& click);
	bool function(bool clear = false);
	bool addChar(char t);
	void setString(std::string y) { text = y; textOutput.setString(y); }
	void checkState();
	void draw() { window.draw(spriteBox); window.draw(textOutput); }
	void setPosition(int x, int y);
	void setTextPosition(int x, int y) { textOutput.setPosition(x, y);  textOutput.setScale(setting.xScale, setting.yScale);};
	void setColor(sf::Color x) { textOutput.setFillColor(x); }
	void setFont(sf::Font& x) { textOutput.setFont(x); }
	void setSize(int x) { textOutput.setCharacterSize(x); }
	bool empty() { return !activated; }
	void activate() { activated = true;  focuse = true;  spriteBox.setTexture(boxOn); };
	bool getFocuse() { return focuse; }
	void disActivate() { focuse = false;  spriteBox.setTexture(boxOff); };
	std::string& getText() { return text; }
	void clear() { text.clear(); textOutput.setString(""); }
	void setScale(double x, double y) { spriteBox.setScale(x, y); }
	void setSoundVolume(double vol) { click.setVolume(vol * 100); }
	void setAlpha(int a) { spriteBox.setColor(sf::Color(255, 255, 255, a)); }
	bool addNumber(char t);
	~inputText();
};


#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "inputText.h"
#include <algorithm>
#include "CheckBox.h"
class setPlayer
{
private:
	static std::vector<sf::Keyboard::Key> forbidden;
	sf::Keyboard::Key leftKey;
	sf::Keyboard::Key rightKey;
	sf::RenderWindow& window;
	inputText nickname;
	inputText left;
	inputText right;
	CheckBox checkBox;
	//sf::Sprite selected;
	int activated;
	std::string translate(sf::Keyboard::Key key);
	sf::Color playerColor;
	static bool focused;
public:
	setPlayer(sf::RenderWindow& win, sf::Texture& boxOn_, sf::Texture& boxOff, sf::Texture& controls, sf::Texture& controlsOff, sf::Texture& checkOn, sf::Texture& checkOff, sf::SoundBuffer& click, sf::Font& font, std::string string);
	bool function();
	void setColor(sf::Color x) { playerColor = x; }
	sf::Color getColor() { return playerColor; }
	bool setChar(sf::Keyboard::Key key);
	void checkState();
	void draw() { right.draw(); left.draw(); nickname.draw(); checkBox.draw(); }
	void setPosition(int x, int y);
	static bool getFocus() { return focused; }
	void setSoundVolume(double vol) { nickname.setSoundVolume(vol); right.setSoundVolume(vol); left.setSoundVolume(vol); checkBox.setSoundVolume(vol); }
	static void clearForbidden() { forbidden.clear(); }
	std::string getNickname();
	sf::Keyboard::Key getLeftKey();
	sf::Keyboard::Key getRightKey();
	bool getActive() { return checkBox.getActivate(); }
	~setPlayer();

};

